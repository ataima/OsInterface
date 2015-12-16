// last revision: 12sep14

//#define PROCMON
//#define _16mar11  // double buffering and dbg_printf1()

//#define malloc dlmalloc
//#define free dlfree


//#define CONSOLED

#ifdef USE_WINDOWS

#define WANTED_VERSION _T("ProductVersion")  // _T("FileVersion")

#ifndef WINCE
#include <tchar.h>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")

#endif

#ifdef CONSOLED
#define dbg_printf_ printf  // removed; define to printf() for debugging
#else
#define dbg_printf_
#endif

#if 0
// GetPrinterDriverDirectory
#include <windows.h>
#include <winspool.h>

#pragma comment(lib, "cmcfg32.lib")
#endif

/*
 * variables you can define
 */
//#define LINUX
//#define KERNELMODE  /* to create a module for kernel drivers */
#ifndef REGISTRYPATH
#define REGISTRYPATH "SOFTWARE\\CTS"
#endif
#ifndef FILENAME
#define FILENAME "c:\\aaa.log"
#endif

/*
 * syntax for dbg_printf():
 *
 *	<b> or <B> ... </b> or </B> or \n -> BOLD
 *	<RRGGBB>...</> or \n -> COLOR
 */

#ifndef KERNELMODE
#ifndef LINUX
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <winioctl.h>  // for PROCMON extensions
#else
#include <platform.h>
#endif

#include <assert.h>
#include "../inc/debug.h"

#ifndef WINCE
#pragma comment(lib, "version.lib")  // VerQueryValue, GetFileVersionInfo...
#endif

// timing stuff (enabled from dbg_init())
void init_time(struct timing *t);
double get_time(struct timing *t, BOOL cumulate);

FILE *fd = 0; // debugging on file for dbg_printf_()
int g_shidx = -1; // set by map_init()
wchar_t MyDir[_MAX_PATH];

#else  //KERNELMODE
#define _X86_
typedef unsigned char BYTE, *LPBYTE;
typedef unsigned long DWORD;
#define FASTCALL _fastcall
#define DECLSPEC_NORETURN __declspec(noreturn)
#define CDECL
typedef unsigned long UINT_PTR, *PUINT_PTR;
#include <wdm.h>
#include <stdarg.h>
#undef wsprintf
#undef wvsprintf
#define wsprintf sprintf
#define wvsprintf vsprintf
#endif

/* defines for the debugging presentation stuff */
#define LEAD  80
#define TRAIL 60  // 80 is too much for WinDbg...
#define RAWDATALEN 2048

#define NAME_LEAD  "Global\\_MAP_"
#define MUTEX_LEAD "Global\\_MTX_"

BYTE debug; // activation flag: RegQueryValue requires a char *
char in_buf[128]; // global for dbg_input

// define & build szBuf[] with or without timing info
// we use a macro here, so we can write "format"
#if _MSC_VER == 1200  // VC6
#define FIX_VARARGS(index)\
	char szBuf[RAWDATALEN+128];\
	va_list arg_ptr;\
	int bias = 0;\
	\
	if (!debug) return;\
	\
	if (modentries[index].t!=NO_TIMING && modentries[index].newline) {\
			/* prepend timing info */\
		double now = get_time(&modentries[index].mytime,\
			modentries[index].t==PARTIAL_TIMES ? FALSE:TRUE);\
		bias = sprintf(szBuf, "<000080>%-15.2f</>\t", now);\
		}\
	\
	va_start(arg_ptr, format);\
	if (vsprintf(szBuf+bias, format, arg_ptr) == -1)\
		sprintf(szBuf+bias, "<...>\n");\
	va_end(arg_ptr);

#define FIX_VARARGS_NOTIME()\
	char szBuf[RAWDATALEN+128];\
	va_list arg_ptr;\
	\
	if (!debug) return;\
	\
	va_start(arg_ptr, format);\
	if (!vsprintf(szBuf, format, arg_ptr) == -1)\
		sprintf(szBuf, "<...>\n");\
	va_end(arg_ptr);
#else
#define FIX_VARARGS(index)\
	char szBuf[RAWDATALEN+128];\
	va_list arg_ptr;\
	int bias = 0;\
	\
	if (!debug) return;\
	\
	if (modentries[index].t!=NO_TIMING && modentries[index].newline) {\
			/* prepend timing info */\
		double now = get_time(&modentries[index].mytime,\
			modentries[index].t==PARTIAL_TIMES ? FALSE:TRUE);\
		bias = sprintf_s(szBuf, RAWDATALEN+128, "<000080>%-15.2f</>\t", now);\
		}\
	\
	va_start(arg_ptr, format);\
	if (vsprintf_s(szBuf+bias, RAWDATALEN+128-bias, format, arg_ptr) == -1)\
		sprintf_s(szBuf+bias, RAWDATALEN+128-bias, "<...>\n");\
	va_end(arg_ptr);

#define FIX_VARARGS_NOTIME()\
	char szBuf[RAWDATALEN+128];\
	va_list arg_ptr;\
	\
	if (!debug) return;\
	\
	va_start(arg_ptr, format);\
	if (!vsprintf_s(szBuf, RAWDATALEN+128, format, arg_ptr) == -1)\
		sprintf_s(szBuf, RAWDATALEN+128, "<...>\n");\
	va_end(arg_ptr);
#endif

// directory of shared memories: first half for Global entries, second half
// for session-local entries
struct modentry modentries[2 * DIR_ENTRIES];
#ifndef _16mar11
char tracebuf[1024 * 256];
#else
char tracebuf1[1024 * 256]/*for even idx*/, tracebuf2[1024 * 256]/*for odd idx*/;

struct thread_map {
    DWORD thread_id;
    int index;
};
struct thread_map threads[10]; // associate a thread id to a banknote index
#endif

// forward defined internal prototypes
static int map_init(LPSTR moduleName);
static BOOL map_enter(int shidx, LPSTR *string);
static void map_leave(int idx);
static BOOL map_put(int shidx, LPSTR data); // used by dbg_printf()
static LPSECURITY_ATTRIBUTES build_SA(void);

#ifdef PROCMON
#define FILE_DEVICE_PROCMON_LOG 0x00009535
#define IOCTL_EXTERNAL_LOG_DEBUGOUT	(ULONG)CTL_CODE(\
	FILE_DEVICE_PROCMON_LOG, 0x81, METHOD_BUFFERED, FILE_WRITE_ACCESS)

// global file handle to the Process Monitor device
static HANDLE g_hDevice = INVALID_HANDLE_VALUE;

void OpenProcessMonitorLogger(void);
void CloseProcessMonitorLogger(void); // unused
BOOL __stdcall ProcMonDebugOutput(LPSTR lpOutputString);
#endif

/*
 * return FALSE if current process cannot open Global sections
 */
BOOL
is_elevated(void) {
    int i, ret;
    HANDLE hMem;
    TCHAR name[DIR_LENGTHS + 12]; // 12 is strlen(NAME_LEAD) "Global\\_MAP_nn"

    // loop looking for a free slot (we assume it exists!)
    for (i = 0;; i++) {
        wsprintf(name, _T(NAME_LEAD) _T("%02d"), i);
        hMem = CreateFileMapping(INVALID_HANDLE_VALUE, 0,
                PAGE_READWRITE | SEC_COMMIT, 0, MAPSIZE + sizeof (struct modname), name);
        ret = GetLastError();
        if (hMem) CloseHandle(hMem);

        if (ret != ERROR_ALREADY_EXISTS) break;
    }

    dbg_printf_("is_elevated GetLastError() %d\n", GetLastError());

    // ERROR_ACCESS_DENIED will be returned even if the map entry is empty
    return ret == ERROR_ACCESS_DENIED ? FALSE : TRUE;
}

/*
 * format into returned LPSTR (static txt), but do not print anywhere
 * "txt" was static, but this gives problems in multi-thread environments
 */
LPSTR CDECL
dbg_output(BYTE *buf, DWORD len) {
    unsigned i;
    char *txt;
    char crt[6];
    unsigned startlen, endlen;
    BYTE c;
    int txtlen = 4 * (LEAD + TRAIL) + 6; // 6: " ... " + null

    txt = malloc(txtlen); // 6: " ... " + null
    txt[0] = 0;

    // build leading string
    // sprintf() seems to produce memory leaks
    startlen = len > LEAD ? LEAD : len;
    for (i = 0; i < startlen; ++i) {
        c = buf[i];
        if (c >= ' ' && c < 0x7f) wsprintfA(crt, "%c", c);
        else wsprintfA(crt, "<%02x>", c & 0xff);
#if _MSC_VER == 1200  // VC6
        strcat(txt, crt);
#else
        strcat_s(txt, txtlen, crt);
#endif
    }

#if _MSC_VER == 1200  // VC6
    if (len > LEAD + TRAIL) strcat(txt, " ... ");
#else
    if (len > LEAD + TRAIL) strcat_s(txt, txtlen, " ... ");
#endif

    // build trailing string
    if (len > startlen) {
        endlen = len - startlen;
        endlen = endlen > TRAIL ? TRAIL : endlen;
        for (i = len - endlen; i < len; ++i) {
            c = buf[i];
            if (c >= ' ' && c < 0x7f) wsprintfA(crt, "%c", c);
            else wsprintfA(crt, "<%02x>", c & 0xff);
#if _MSC_VER == 1200  // VC6
            strcat(txt, crt);
#else
            strcat_s(txt, txtlen, crt);
#endif
        }
    }

    return txt;
}

#if 0

/*
 * internal old-style dbg_printf()
 */
void CDECL
dbg_printf_(LPSTR format, ...) {
    FIX_VARARGS(g_shidx);

#ifndef KERNELMODE
    if (debug & 1) OutputDebugString(szBuf);
    if (debug & 2 && fd > 0) {
        fwrite(szBuf, strlen(szBuf), 1, fd);
        fflush(fd);
    }
#else
    DbgPrint(szBuf);
#endif
}
#endif

// for debugging dbgctrl.c
//extern void CDECL DBG_PRINTF(LPSTR format, ...);
//#  define dbg_printf_ DBG_PRINTF   // define to printf() for debugging

void CDECL
dbg_printf(LPSTR format, ...) {
    FIX_VARARGS(g_shidx); // define & build szBuf[] with or without timing info

#ifndef KERNELMODE
    map_put(g_shidx, szBuf);
#ifdef PROCMON
    if (g_hDevice != INVALID_HANDLE_VALUE) ProcMonDebugOutput(szBuf);
#endif
#endif//KERNELMODE
}

void CDECL
dbg_printf_multi(int shidx, LPSTR format, ...) {
    FIX_VARARGS(shidx);
    if (shidx == -1) return;

#ifndef KERNELMODE
    map_put(shidx, szBuf);

#ifdef PROCMON
    if (g_hDevice != INVALID_HANDLE_VALUE) ProcMonDebugOutput(szBuf);
#endif
#endif//KERNELMODE
}

#ifdef _16mar11

/*
 * added on 10dec10, to limit the big mass of data
 */
void CDECL
dbg_printf1(LPSTR format, ...) {
    extern int do_trace;

    FIX_VARARGS(g_shidx);

    if (do_trace & 4) map_put(g_shidx, szBuf);
    else {
        int i;
        DWORD myId = GetCurrentThreadId();

        // search the banknote index associated with myId
        for (i = 0; i < 10; i++) if (threads[i].thread_id == myId) break;
        if (i == 10) {
            dbg_printf("<FF0000>AAARGH! dbg_printf1(%s) - myId %x\n",
                    format, myId);
            dbg_printf("threads[]: ");
            for (i = 0; i < 10; i++) if (threads[i].thread_id)
                    dbg_printf("%d-%x %d, ",
                        i, threads[i].thread_id, threads[i].index);
            dbg_printf("\n");
        }

#if _MSC_VER == 1200  // VC6
        if ((threads[i].index & 1) == 0) strcat(tracebuf1, szBuf);
        else strcat(tracebuf2, szBuf);
#else
        if ((threads[i].index & 1) == 0)
            strcat_s(tracebuf1, sizeof (tracebuf1), szBuf);
        else strcat_s(tracebuf2, sizeof (tracebuf2), szBuf);
#endif
    }
}
#endif//_16mar11

void CDECL
dbg_trace_multi(int shidx, BYTE *buf, DWORD len, LPSTR format, ...) {
    char *txt;
    FIX_VARARGS_NOTIME();
    UNREFERENCED_PARAMETER(shidx);

    // we want only one timing info!
    txt = dbg_output(buf, len);
    //dbg_printf_multi(shidx, "%s %s\n", szBuf, txt);
    free(txt);
}

void CDECL
dbg_trace(BYTE *buf, DWORD len, LPSTR format, ...) {
    char *txt;
    FIX_VARARGS_NOTIME();

    txt = dbg_output(buf, len);
    dbg_printf("%s %s\n", szBuf, txt);
    free(txt);
}

#ifndef KERNELMODE

void CDECL
dbg_error(LPSTR api, int error_type) {
    dbg_error_multi(g_shidx, api, error_type);
}

/*
 * 'error_type' may be the last error, if null, or the value itself
 */
void
dbg_error_multi(int shidx, LPSTR api, int error_type) {
    ULONG errorCode;
    TCHAR errorBuffer[512]; // extended for WSA errors
    char *buf;
    TCHAR *p;

    if (!debug) return;

    errorCode = error_type ? error_type : GetLastError();

    if (!FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorCode, 0,
            errorBuffer, sizeof (errorBuffer) / sizeof (TCHAR), NULL)) {
        dbg_printf(RED"<b>FormatMessage() error %d\n", GetLastError());
        dbg_printf_multi(shidx, RED"Error in %s(): %u\n", api, errorCode);
    } else {
        buf = malloc(160);

        // take off ^M
        for (p = &errorBuffer[_tcslen(errorBuffer) - 1]; *p == 0x0d || *p == 0x0a; p--)
            *p = 0;

        dbg_printf_multi(shidx, RED"Error in %s(): %u (%s)\n",
                api, errorCode, GENERIC_TO_ASCII(errorBuffer, buf, 160));
        free(buf);
    }
    modentries[shidx].newline = TRUE;
}

/*
 * "shidx" can be 0/DIR_ENTRIES(10)-1 for Global (elevated) entries, and
 *     DIR_ENTRIES+0/DIR_ENTRIES+DIR_ENTRIES-1 for session-local entries
 * "mep" is returned with info on shared memory
 * "in_memory" tell to maintain the shared memory in process and create
 *     a proper mutex for accessing it
 *
 * return FALSE if some unexpected error happened
 */
BOOL
get_shared_entry(int shidx, struct modentry *mep, BOOL in_memory) {
    int ret;
    HANDLE hMem;
    LPBYTE shmem;
    TCHAR name[DIR_LENGTHS + 12]; // 12 is strlen(NAME_LEAD) "Global\\_MAP_nn"

    // build "name" according to given index and pertaining namespace:
    // "Global\_MAP_nn" (=>\BaseNamedObjects\...) for a Session 0 process,
    // "_MAP_nn" (\Sessions\1\BaseNamedObjects\...) for a local not-service
    // process
    wsprintf(name,
            _T(NAME_LEAD)_T("%02d") + (shidx < DIR_ENTRIES ? 0 : 7), shidx);

    hMem = CreateFileMapping(INVALID_HANDLE_VALUE, 0, PAGE_READWRITE | SEC_COMMIT,
            0, MAPSIZE + sizeof (struct modname), name);
    ret = GetLastError();

    if (hMem && ret == ERROR_ALREADY_EXISTS) {
        // sharedmem present: bring it into our memory, for reading the name
        shmem = MapViewOfFile(hMem, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        if (!shmem) {
            dbg_printf_(
                    "get_shared_entry(%d) ko: MapViewOfFile() error %d\n",
                    shidx, GetLastError());
            CloseHandle(hMem);
            return FALSE;
        }

        // store its name into the passed "mep"
#if _MSC_VER == 1200  // VC6
        strncpy(mep->name,
                ((struct modname *) (shmem + MAPSIZE))->name, DIR_LENGTHS);
#else
        strncpy_s(mep->name, DIR_LENGTHS,
                ((struct modname *) (shmem + MAPSIZE))->name, DIR_LENGTHS);
#endif

        if (!in_memory) {
            // clean-out
            UnmapViewOfFile(shmem);
            CloseHandle(hMem);
            return TRUE;
        }

        // else leave the entry in memory: store info for dbg_end()
        mep->hshmem = hMem;
        mep->base = shmem;

        // we need a parallel mutex too
        wsprintf(name,
                _T(MUTEX_LEAD)_T("%02d") + (shidx < DIR_ENTRIES ? 0 : 7), shidx);
#ifndef WINCE
        mep->hmutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, name); // existing
#else
        mep->hmutex = CreateMutex(0, FALSE, name);
#endif
        if (!mep->hmutex) {
            dbg_printf_(
                    "get_shared_entry(%d): OpenMutex() failure (error %d)\n",
                    shidx, GetLastError());
            return FALSE;
        }
        return TRUE;
    }

    if (ret && ret != ERROR_ACCESS_DENIED/*not elevated*/) {
        // some bad error happened
        dbg_printf_("get_shared_entry(%d) ko: CreateFileMapping() error %d\n",
                shidx, ret);
        if (hMem) CloseHandle(hMem);
        return FALSE;
    }

    // !ret || ERROR_ACCESS_DENIED
    // sharedmem not present in global or session1 space
    if (hMem) CloseHandle(hMem);
    if (ret == ERROR_ACCESS_DENIED) {
        //assert(in_memory == FALSE);
        mep->name[0] = '@'; // forbidden when creating new entries
    }

    return TRUE; // no bad errors, entry not found
}

/*
 * called only by map_init() to create a new shared memory
 */
BOOL
set_shared_mem(int idx, LPSTR moduleName) {
    HANDLE hMem, hMutex;
    LPBYTE shmem;
    TCHAR name[DIR_LENGTHS + 12]; // 12 is strlen(NAME_LEAD)

    // build "name" according to given index and pertaining namespace:
    // global for a Session 0 process, local for a not-service process
    wsprintf(name, _T(NAME_LEAD)_T("%02d") + (idx < DIR_ENTRIES ? 0 : 7), idx);

    // can be already existing, but in that case it returns an handle too
    hMem = CreateFileMapping(INVALID_HANDLE_VALUE,
            build_SA() /*security*/,
            PAGE_READWRITE | SEC_COMMIT,
            0, MAPSIZE + sizeof (struct modname), // high and low size
            name);
    if (!hMem) {
        dbg_printf_(
                "set_shared_mem(%d): CreateFileMapping() ko (error %d)\n",
                idx, GetLastError());
        dbg_end_multi(idx);
        return FALSE;
    }

    // get the shared memory local address
    shmem = MapViewOfFile(hMem, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if (!shmem) {
        dbg_printf_(
                "set_shared_mem(%d): MapViewOfFile() ko (error %d)\n",
                idx, GetLastError());
        dbg_end_multi(idx);
        return FALSE;
    }

    //  bring our module name into shared memory
#if _MSC_VER == 1200  // VC6
    strncpy(((struct modname *) (shmem + MAPSIZE))->name,
            moduleName, DIR_LENGTHS);
#else
    strncpy_s(((struct modname *) (shmem + MAPSIZE))->name, DIR_LENGTHS,
            moduleName, DIR_LENGTHS);
#endif

    // create the associated mutex
    wsprintf(name, _T(MUTEX_LEAD)_T("%02d") + (idx < DIR_ENTRIES ? 0 : 7), idx);
    hMutex = CreateMutex(build_SA(), FALSE, name); // can be already existing
    if (!hMutex) {
        dbg_printf_("set_shared_mem(%d): CreateMutex() ko (error %d)\n",
                idx, GetLastError());
        dbg_end_multi(idx);
        return FALSE;
    }

    // initialize info for dbg_end()
#if _MSC_VER == 1200  // VC6
    strncpy(modentries[idx].name,
            ((struct modname *) (shmem + MAPSIZE))->name, DIR_LENGTHS);
#else
    strncpy_s(modentries[idx].name, DIR_LENGTHS,
            ((struct modname *) (shmem + MAPSIZE))->name, DIR_LENGTHS);
#endif

    modentries[idx].hshmem = hMem;
    modentries[idx].base = shmem;
    modentries[idx].hmutex = hMutex;

    dbg_printf_("set_shared_mem(%d) ok\n", idx);
    return TRUE;
}

int CDECL
dbg_init(char *modname, enum timing_mode t) {
    TCHAR szFileName[_MAX_PATH] = {0};
    MEMORY_BASIC_INFORMATION mbi = {0};
    HMODULE hMod = 0;
    char buf[_MAX_PATH];
    wchar_t wbuf[_MAX_PATH];
    LPVOID curaddr; // to be made global for LINUX asm

#if 0  //WAS ifndef LINUX
    {
        HKEY hkey = 0;
        DWORD tmp = sizeof (int);
        MEMORY_BASIC_INFORMATION mbi = {0};

        /* test debug flag */
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_READ, &hkey) ||
                RegQueryValueEx(hkey, "debug", 0, 0, &debug, &tmp)) debug = 0;
        if (hkey) RegCloseKey(hkey); // just to be polite...

#if 0
        if (debug & 2) {
            //unlink(FILENAME);
            fd = fopen(FILENAME, "a+");
        }
#endif

        // trick to retrieve the current module handle
        // for linux hMod=0 is enough!
        __asm{
            call $ + 5
            pop eax // get EIP into EAX
            mov curaddr, eax
        }
        VirtualQuery(curaddr, &mbi, sizeof (mbi)); // fill "mbi"
        hMod = mbi.AllocationBase;

        if (newmap == FALSE) hMod = 0;
    }
#else
    debug = 1;
#endif//LINUX

    // (re)open the associated shared memory
    g_shidx = map_init(modname);
    if (g_shidx == -1) return -1;

    if (!GetModuleFileName(hMod/*0*/, szFileName, sizeof (szFileName))) {
        dbg_printf_("GetModuleFileName_1() ko: dbg_init() does not operate\n");
        return -1;
    }

    // these prints will already go on the sharedmem
    dbg_printf(
            "<b>dbg_init()</b> called by module \"%s\", pid %#x, version \"%s\"\n",
            GENERIC_TO_ASCII(szFileName, buf, _MAX_PATH),
            GetCurrentProcessId(), dbg_version(hMod));
    //dbg_printf("g_shidx=%d, shmem=%x\n", g_shidx, modentries[g_shidx].base);

    // trick to retrieve the current module handle
#if 1
    curaddr = _ReturnAddress();
#else
    //__asm {
    //call $+5
    //pop eax  // get EIP into EAX
    //mov curaddr, eax
    //}
#endif

    VirtualQuery(curaddr, &mbi, sizeof (mbi)); // fill "mbi"
    hMod = mbi.AllocationBase;
    if (!GetModuleFileName(hMod, szFileName, sizeof (szFileName))) {
        dbg_printf("GetModuleFileName_1() ko: dbg_init() does not operate\n");
        return -1;
    }
    dbg_printf(
            "<b>current module:</b> \"%s\", version: \"%s\", g_shidx: %d\n",
            GENERIC_TO_ASCII(szFileName, buf, _MAX_PATH),
            dbg_version(hMod), g_shidx);

#if 1
    // set MyDir[]
    // this way, the result is "/Windows/system32/spool/DRIVERS/x64/3/"
    // or "/Windows/system32/spool/DRIVERS/w32x86/3/"
#if _MSC_VER == 1200  // VC6
    _wsplitpath(GENERIC_TO_UNICODE(szFileName, wbuf, _MAX_PATH),
            MyDir, // drive
            MyDir + 2,
            0, // fname
            0); // ext
#else
    if (_wsplitpath_s(GENERIC_TO_UNICODE(szFileName, wbuf, _MAX_PATH),
            MyDir, 2 + 1, // drive and size
            MyDir + 2, _MAX_PATH - 2,
            0, 0, // fname
            0, 0)) { // ext
        dbg_printf(RED"_splitpath_s() ko\n");
        wcscpy_s(MyDir, _MAX_PATH, wbuf);
    }
#endif

#else  // NOT WORKING IN LANGUAGE MONITOR: SPOOLSV.EXE HANGS!
    {
        // this way result is "C:\Windows\system32\spool\DRIVERS\x64"
        DWORD uSize = MAX_PATH;

        if (GetPrinterDriverDirectory(NULL, // local server name
                _T("Windows x64"), // environment
                1, // structure level must be 1
                (LPBYTE) MyDir, MAX_PATH, &uSize))
        }
#endif

#ifdef PROCMON
    OpenProcessMonitorLogger();
#endif

    modentries[g_shidx].t = t;
    if (t) init_time(&modentries[g_shidx].mytime);

    modentries[g_shidx].newline = TRUE;
    return g_shidx;
}
#endif//KERNELMODE

#if 0

/*
 * to be used only by console applications...
 * return the number of read characters, and the buffer in_buf[]
 * hex bytes are to be inserted as "\NN", plain '\' must be doubled
 */
int
dbg_input(char *prompt) {
    char c, mybuf[128], *mybufp = mybuf;
    int i, bufptr = 0;

    *in_buf = 0; // just in case
    printf("%s", prompt); // output on console
    if (!gets(mybuf)) return 0;

    while ((c = *mybufp) != 0) {
        if (*mybufp == '\\') {

            if (*++mybufp == '\\') {
                in_buf[bufptr++] = '\\';
                mybufp++;
                continue;
            }

            if (!sscanf(mybufp, "%02x", &i)) return 0;
            if (i > 0xff) return 0; // cannot happen!
            in_buf[bufptr++] = (char) i;
            mybufp += 2; // update mybufp
            continue;
        }
        // that's the plain case
        in_buf[bufptr++] = c;
        mybufp++;
    }

    in_buf[bufptr] = 0; // just in case
    return bufptr;
}
#endif

#ifndef KERNELMODE

/*
 * if arg is 0, the returned version will be taken from this executable
 */
LPSTR CDECL
dbg_version(HMODULE hMod) {
#ifndef LINUX
    static TCHAR buffer[256];
    static char nul[] = "unknown";
    static char ret[256];
    DWORD dwVerInfoSize, dwVerHwnd /*will be set to 0*/;
    LPTSTR versionInfo = 0;
#if 0
    DWORD dwMajorVersion, dwMinorVersion, dwBuildNumber;
#endif
    char *buf;

    GetModuleFileName(hMod/*WAS 0*/, buffer, 256);

    if ((dwVerInfoSize = GetFileVersionInfoSize(buffer, &dwVerHwnd)) == 0) {
        //dbg_error(_T("GetFileVersionInfoSize"), 0);  // cant reach DLL file
        return nul; // avoid null to avoid traps...
    }

    // alloc memory for fileinfo
    if ((versionInfo = (LPTSTR) malloc(dwVerInfoSize)) == 0) {
        dbg_printf("malloc(%d) failed!\n", dwVerInfoSize);
        return nul; // avoid null to avoid traps...
    }

    // try to get the info into versionInfo
    if (!GetFileVersionInfo(buffer, dwVerHwnd, dwVerInfoSize, versionInfo)) {
        // weird, since we could read the information size...
        dbg_error("GetFileVersionInfo", 0);
        free(versionInfo);
        return nul; // avoid null to avoid traps...
    }

    /*
     * parsification of the buffer versionInfo
     */
    {
        DWORD langD;
        unsigned int vLen;
        LPTSTR version = 0;

        // fill langD for the following interrogation
        wsprintf(buffer, _T("\\VarFileInfo\\Translation"));
        if (!VerQueryValue(versionInfo, buffer, (PVOID*) & version, &vLen)) {
            dbg_printf("VerQueryValue(\\VarFileInfo\\Translation) failed\n");
            free(versionInfo);
            return nul; // avoid null to avoid traps...
        }

        // replace buffer for the following parsification
        if (vLen == 4) {
            /* The returned 'hex' value looks a little confusing, but
            essentially what it is, is the hexidecimal representation
            of a couple different values that represent the language and
            character set that we are wanting string values for.
            040904E4 is a very common one, because it means:
            US English, Windows MultiLingual characterset
            Or to pull it all apart:
            04------        = SUBLANG_ENGLISH_USA
            --09----        = LANG_ENGLISH
            ----04E4 = 1252 = Codepage for Windows:Multilingual */
            memcpy(&langD, version, 4);
            wsprintf(buffer,
                    _T("\\StringFileInfo\\%02X%02X%02X%02X\\")WANTED_VERSION,
                    (langD & 0xff00) >> 8, langD & 0xff,
                    (langD & 0xff000000) >> 24, (langD & 0xff0000) >> 16);
        } else wsprintf(buffer, _T("\\StringFileInfo\\%04X04B0\\")WANTED_VERSION,
                GetUserDefaultLangID());

        buf = malloc(128);

        if (!VerQueryValue(versionInfo, buffer, (PVOID*) & version, &vLen)) {
            dbg_printf("VerQueryValue(%s) failed\n",
                    GENERIC_TO_ASCII(version, buf, 128));
            free(versionInfo);
            free(buf);
            return nul; // avoid null to avoid traps...
        }
        // truncate it!
#if _MSC_VER == 1200  // VC6
        strncpy(ret, GENERIC_TO_ASCII(version, buf, 128), sizeof (ret) - 1);
#else
        strncpy_s(ret, 256, GENERIC_TO_ASCII(version, buf, 128), sizeof (ret) - 1);
#endif
        ret[sizeof (ret) - 1] = 0;
        free(buf);
    }

    free(versionInfo);
    return ret;
#else
    return "";
#endif
}
#endif//KERNELMODE

#if 0

BOOL
SetPrivilege(
        LPCTSTR lpszPrivilege, // name of privilege to enable/disable
        BOOL bEnablePrivilege // to enable or disable privilege
        ) {
    HANDLE hToken, // access token handle
            TOKEN_PRIVILEGES tp;
    LUID luid;

    OpenProcessToken(GetCurrentProcess, TOKEN_ADJUST_PRIVILEGES Or TOKEN_QUERY, hToken)
    SetPrivilege(hToken...)
    CloseHandle(hToken)

    if (!LookupPrivilegeValue(
            NULL, // lookup privilege on local system
            lpszPrivilege, // privilege to lookup
            &luid)) // receives LUID of privilege
    {
        dbg_printf_("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof (TOKEN_PRIVILEGES),
            (PTOKEN_PRIVILEGES) NULL,
            (PDWORD) NULL)) {
        dbg_printf_("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        dbg_printf_("The token does not have the specified privilege. \n");
        return FALSE;
    }


    return TRUE;
}
#endif

/*
 * If NULL is specified for LPSECURITY_ATTRIBUTES when creating an object,
 * the object is assigned the default security descriptor of the calling
 * process. This is not the same as granting access to everyone by assigning
 * a NULL discretionary access control list (DACL). The default security
 * descriptor is based on the default DACL of the access token belonging
 * to the calling process. By default, the default DACL in the access token
 * of a process allows access only to the user represented by the access
 * token.
 * In order to allow other users to access the object, this function creates
 * a security descriptor with the appropriate access.
 */
static LPSECURITY_ATTRIBUTES
build_SA(void) {
#ifdef WINCE
    return 0;
#else
    static SECURITY_ATTRIBUTES sa;
    PSECURITY_DESCRIPTOR pSD;

    if (sa.nLength) return &sa; // no need to build 'sa' more than once

    // create a security descriptor that allows anyone to access the object
    pSD = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (!pSD) {
        dbg_printf_("LocalAlloc pSD failed");
        return 0;
    }

    if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION)) {
        dbg_printf_("InitializeSecurityDescriptor failed");
        LocalFree(pSD);
        return 0;
    }

    // add a NULL discretionary ACL to our security descriptor
    if (!SetSecurityDescriptorDacl(pSD, TRUE, NULL, FALSE)) {
        dbg_printf_("SetSecurityDescriptorDacl failed");
        LocalFree(pSD);
        return 0;
    }

    // build the security attributes to be returned
    sa.nLength = sizeof (sa);
    sa.lpSecurityDescriptor = pSD;
    sa.bInheritHandle = FALSE;

    return &sa;
#endif//WINCE
}

/*
 * called only by dbg_init(): create/reopen a sharedmem + mutex slot
 */
static int
map_init(LPSTR moduleName_) {
    BOOL ret;
    int i, firstempty = -1;
    char moduleName[DIR_LENGTHS];

    // refresh all modentries[] names
    if (!dir_get()) return -1;

#if _MSC_VER == 1200  // VC6
    strcpy(moduleName, moduleName_);
    if (is_elevated()) strcat(moduleName, "*");
#else
    strcpy_s(moduleName, DIR_LENGTHS, moduleName_);
    if (is_elevated()) strcat_s(moduleName, DIR_LENGTHS, "*");
#endif

    // list all the entries, to see if we already have some entry
    // assigned to us
    for (i = 0; i < 2 * DIR_ENTRIES; i++) {
        if (!strncmp(modentries[i].name, moduleName, DIR_LENGTHS)) {
            // found a sharedmem which is already assigned to us:
            // bring it in memory (HERE without dbg_end())
            ret = get_shared_entry(i, &modentries[i], TRUE);
            if (!ret) {
                dbg_printf_("map_init(): get_shared_entry() ko\n");
                return -1;
            }

            dbg_printf_("modname %s already present in directory at slot %d\n",
                    moduleName, i);
            return i;
        }

        // save the first empty index in case the entry is not found
        if (!modentries[i].name[0] && firstempty == -1) firstempty = i;
    }

    if (firstempty == -1) {
        dbg_printf_("no space! map_init() returns -1\n");
        return -1;
    }

    // not found: we must create a new sharedmem at position 'firstempty'
    dbg_printf_("adding module name %s at slot %d\n", moduleName, firstempty);
    if (!set_shared_mem(firstempty, moduleName)) return -1;
    return firstempty;
}

/*
 * write to shared memory, using a mutex as mechanism for mutual exclusion
 */
static BOOL
map_put(int shidx, LPSTR data) {
    DWORD lentot = (DWORD) strlen(data);
    DWORD lenfirst;
    DWORD bias;
    LPSTR pBuffer;

    if (shidx == -1) return FALSE;

    if (!map_enter(shidx, &pBuffer)) {
        dbg_error("WaitForMutex_put", 0);
        return FALSE; // may happen during the first dbg_printf()
    }

    // read the last 'bias'
    bias = ((struct modname *) (pBuffer + MAPSIZE))->bias;
    //dbg_printf_("W<%x\n", bias);

    // check for wraparound
    // this may divide our 'data' in 2 chunks: lenfirst + (lentot-lenfirst)
    lenfirst = lentot > MAPSIZE - bias ? MAPSIZE - bias : lentot;

    // write the first chunk
    memcpy(pBuffer + bias, data, lenfirst);

    // update 'bias' and possibly write the second chunk
    if (lenfirst == lentot) bias += lenfirst; // only one chunk was necessary
    else {
        // dump the second chunk at start of the shared buffer
        bias = lentot - lenfirst;
        memcpy(pBuffer, data + lenfirst, bias);
    }

    // bias will never point after the last buffer byte
    if (bias == MAPSIZE) bias = 0;

    // after every put, update 'bias' in the shared memory
    ((struct modname *) (pBuffer + MAPSIZE))->bias = bias;
    //dbg_printf_("W>%x\n\n", bias);

    map_leave(shidx); // release the associated mutex
    modentries[shidx].newline = data[lentot - 1] == '\n' ? TRUE : FALSE;
    return TRUE;
}

void CDECL
dbg_end(void) {
    char *buf;
    TCHAR szFileName[_MAX_PATH] = {0};

    if (GetModuleFileName(0, szFileName, sizeof (szFileName))) {
        buf = malloc(128);

        dbg_printf("<b>dbg_end()</b> called by module \"%s\"\n",
                GENERIC_TO_ASCII(szFileName, buf, 128));

        dbg_end_multi(g_shidx);
        free(buf);
    }
}

void
dbg_end_multi(int shidx) {
    if (modentries[shidx].base)
        if (!UnmapViewOfFile(modentries[shidx].base))
            dbg_printf_("UnmapViewOfFile() ko\n");

    if (modentries[shidx].hshmem)
        if (!CloseHandle(modentries[shidx].hshmem))
            dbg_printf_("CloseHandle_hMap ko\n");

    if (modentries[shidx].hmutex)
        if (!CloseHandle(modentries[shidx].hmutex))
            dbg_printf_("CloseHandle_hMutex ko\n");

    // clear all previous fields
    memset(&modentries[shidx], 0, sizeof (struct modentry));
}

/*
 * return the pointer to the corresponding memory area, or -1 on error
 * (used by both map_put() and map_get())
 * 03maj11: added the case that the handles are yet null in table
 */
static BOOL
map_enter(int shidx, LPSTR *pBuffer) {
    if (!modentries[shidx].hmutex) {
        dbg_printf_("map_enter(%d): entry not in memory!\n", shidx);
        if (!get_shared_entry(shidx, &modentries[shidx], TRUE))
            return FALSE;
    }

    // enter the critical region
    if (WaitForSingleObject(modentries[shidx].hmutex, INFINITE)) return FALSE;

    *pBuffer = modentries[shidx].base;
    return TRUE;
}

/*
 * simply release the associated mutex, but do not clear its field
 */
static void
map_leave(int shidx) {
    ReleaseMutex(modentries[shidx].hmutex);
}

/*
 * copy the shared buffer into 'mybuf' with appropriate critical regions
 *
 * 'mybuf' must be at least MAPSIZE+1(for the null) bytes long, and that
 * value must be put into *psz before invoking this function
 *
 * return: -1, or 0, or <>0 (buffer wrapped-around)
 */
long
map_get(int shidx, LPSTR mybuf, LPDWORD psz, BOOL from_sot) {
    DWORD bias;
    DWORD i, size, start, to_end;
    LPSTR pBuffer;

    if (!map_enter(shidx, &pBuffer)) {
        //dbg_printf_("R ret -1 (waiting)\n");
        return -1;
    }

    // read the pointer to new space (at end+1)
    bias = ((struct modname *) (pBuffer + MAPSIZE))->bias;

    if (!from_sot) {
        // calculate the whole data size: assume that two consecutive nulls
        // will be found only if the buffer never wrapped around
        if (*(pBuffer + bias) == 0 && *(pBuffer + (bias == MAPSIZE - 1 ? 0 : bias + 1)) == 0) {
            size = bias;
            bias = 0; // adjust pointer to start
            //dbg_printf_("R<%x\n", bias);
        } else {
            size = MAPSIZE; // assume overflow when two nulls not found
            //dbg_printf_("R<%x\n", bias);
        }
    } else {
        start = modentries[shidx].sot;
        goto found;
    }

    // check the size of the caller buffer
    if (*psz < size + 1/*take the trailing null into account*/) {
        // ko: caller buffer too little
        map_leave(shidx); // release the associated mutex
        //dbg_printf_("R ret -1 (caller buffer too little)\n");
        return -1;
    }

    if (size < MAPSIZE && bias == 0) { // no wrap, no first partial line
        memcpy(mybuf, pBuffer, size);
        *psz = size;
        //dbg_printf_("R from start, sz %x\n", *psz);
        // done...
    } else {
        // size==MAPSIZE || bias!=0 => wrapped around
        // look for the start of the first line in two steps
        // first step: search from bias to buffer_end
        to_end = MAPSIZE - bias;
        for (i = 0; i < to_end; i++) if (*(pBuffer + bias + i) == '\n') {
                // found a new line: first line will follow
                start = bias + i + 1;
                if (start == MAPSIZE) start = 0;
                goto found;
            }
        // second step if not yet found: look in the first half of buffer
        // (from 0 to bias)
        for (i = 0; i < bias; i++) if (*(pBuffer + i) == '\n') {
                start = i + 1;
                goto found;
            }

        // ko: newline not found
        *psz = 0;
        map_leave(shidx); // release the associated mutex
        //dbg_printf_("R ret -1 (newline not found)\n");
        return -1;

found:
        if (start > bias) {
            // first copy: prepend a first chunk up to the end of the buffer
            memcpy(mybuf, pBuffer + start, MAPSIZE - start);
            *psz = MAPSIZE - start;
            //dbg_printf_("R1 from %x to end, sz %x\n", start, *psz);
            // second copy: from start of shared memory
            memcpy(mybuf + MAPSIZE - start, pBuffer, bias);
            *psz += bias;
            //dbg_printf_("R2 from 0 to bias %x, sz %x\n", bias, bias);
        } else {
            memcpy(mybuf, pBuffer + start, bias - start);
            *psz = bias - start;
            //dbg_printf_("R3 from start %x to bias %x, sz %x\n",
            //start, bias, *psz);
        }
        size = *psz;
    }

    // done
    mybuf[size] = 0; // just in case

    if (!from_sot) {
        // reset buffer contents and bias - 23dec09
        ZeroMemory(pBuffer, MAPSIZE);
        ((struct modname *) (pBuffer + MAPSIZE))->bias = 0;
        //dbg_printf_("R>0\n\n");
    }

    map_leave(shidx); // release the associated mutex
    return bias;
}

BOOL
map_clean(int shidx) {
    LPSTR pBuffer;

    if (!map_enter(shidx, &pBuffer)) return FALSE;

    // reset buffer contents and bias
    ZeroMemory(pBuffer, MAPSIZE);
    ((struct modname *) (pBuffer + MAPSIZE))->bias = 0;

    map_leave(shidx); // release the associated mutex
    return TRUE;
}

/*
 * update the modentries[] array;
 * note: a shared directory presence is not necessary
 * note: do not touch own entry (if dbg_init() was called)
 */
BOOL
dir_get(void) {
    int i, ret;

    // clear in-memory directory, just in case...
    for (i = 0; i < 2 * DIR_ENTRIES; i++) if (i != g_shidx) dbg_end_multi(i);

    // rebuild the global array modentries[]
    dbg_printf_("dir_get():\n");
    for (i = 0; i < 2 * DIR_ENTRIES; i++) if (i != g_shidx) {
            ret = get_shared_entry(i, &modentries[i]/*OUT*/,
                    FALSE); // do not load in memory
            dbg_printf_("\t%d: ret %d, name: %s\n",
                    i, ret, modentries[i].name[0] ? modentries[i].name : "(null)");
            if (!ret) return FALSE;
        }

    return TRUE;
}

/*
 * set start-of-trace
 */
void
dbg_set_sot(int idx) {
    LPSTR pBuffer;

    if (map_enter(idx, &pBuffer)) {
        modentries[idx].sot = ((struct modname *) (pBuffer + MAPSIZE))->bias;
        map_leave(idx); // release the associated mutex
    }
}

/*
 * get encrypt trace, ret len or -1
 */
int
dbg_get_trc(int idx, LPSTR *mybuf, BOOL from_sot) {
    int i;
    int buflen = MAPSIZE;
    static char mybuf_[MAPSIZE + 1];

    if (map_get(idx, mybuf_, (LPDWORD) & buflen, from_sot) == -1) return -1;

    for (i = 0; i < buflen; i++) mybuf_[i] ^= 0x5A;

    *mybuf = mybuf_;
    return buflen;
}

#ifdef PROCMON

void
OpenProcessMonitorLogger(void) {
    g_hDevice = CreateFile(_T("\\\\.\\Global\\ProcmonDebugLogger"),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

/*
 * close the handle to the driver
 */
void
CloseProcessMonitorLogger(void) {
    if (g_hDevice != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hDevice);
        g_hDevice = INVALID_HANDLE_VALUE;
    }
}

BOOL __stdcall
ProcMonDebugOutput(LPSTR lpOutputString) {
    BOOL bRet = FALSE;
    DWORD iLen, iOutLen;
    LPWSTR lpszW;

    if (g_hDevice == INVALID_HANDLE_VALUE) {
        SetLastError(ERROR_BAD_DRIVER);
        return FALSE;
    }

    if (!lpOutputString) {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    // first call
    iLen = MultiByteToWideChar(CP_ACP, 0, lpOutputString, -1, 0, 0);

    lpszW = malloc(iLen * sizeof (WCHAR));
    // second call
    MultiByteToWideChar(CP_ACP, 0, lpOutputString, -1, lpszW, iLen);

    bRet = DeviceIoControl(g_hDevice, IOCTL_EXTERNAL_LOG_DEBUGOUT,
            lpszW, iLen * sizeof (WCHAR), NULL, 0, &iOutLen, NULL);
    if (bRet == FALSE) {
        DWORD dwLastError = GetLastError();
        // if the driver is loaded but the user mode Process Monitor
        // program is not running, turn the last error into a write  failure
        if (dwLastError == ERROR_INVALID_PARAMETER)
            SetLastError(ERROR_WRITE_FAULT);
    }

    free(lpszW);
    return bRet;
}
#endif

// these two functions want a preallocated output buffer!

LPWSTR
to_unicode(LPSTR src, LPWSTR dst, unsigned dstsize) {
    unsigned i;

    if (strlen(src) < dstsize) dstsize = (int) strlen(src);
    for (i = 0; i < dstsize; i++) *(dst + i) = src[i];
    dst[i] = 0; // do not forget the trailing null

    return dst;
}

LPSTR
to_ascii(LPWSTR src, LPSTR dst, unsigned dstsize) {
    unsigned i;

    if (wcslen(src) < dstsize) dstsize = (int) wcslen(src);
    for (i = 0; i < dstsize; i++) *(dst + i) = (char) src[i];
    dst[i] = 0; // do not forget the trailing null

    return dst;
}

/*
 * initialize first time liT1
 */
void
init_time(struct timing *t) {
    if (!t) t = &modentries[g_shidx].mytime;

    QueryPerformanceFrequency(&t->liFreq); // counts per second
    QueryPerformanceCounter(&t->Tbase);
    t->Tprev = t->Tbase;
}

/*
 * get the following time and return the partial/total time in msecs
 */
double
get_time(struct timing *t, BOOL cumulate) {
    double deltatime;

    QueryPerformanceCounter(&t->Tcur);

    t->liDiff.QuadPart = cumulate ?
            t->Tcur.QuadPart - t->Tbase.QuadPart : t->Tcur.QuadPart - t->Tprev.QuadPart;
    t->liDiff.QuadPart *= 1000;

    // update prev time field
    t->Tprev = t->Tcur;

    deltatime = (double) t->liDiff.QuadPart / t->liFreq.QuadPart;

    return deltatime;
}

LPSTR
show_flags(UINT flg, char *legenda[]/*LSB first*/) {
    int i = 0;
    BOOL add_pipe = FALSE;
    static char flags[256];

    for (i = 0, flags[0] = 0; flg; i++, flg >>= 1) if (flg & 1) {
#if _MSC_VER == 1200  // VC6
            if (add_pipe) strcat(flags, " | ");
            strcat(flags, legenda[i]);
#else
            if (add_pipe) strcat_s(flags, 256, " | ");
            strcat_s(flags, 256, legenda[i]);
#endif
            add_pipe = TRUE;
        }
    return flags;
}


#endif