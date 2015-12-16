// heavily rewritten on 18oct10-20oct10 by rc
// renewed for Matica on 14jan13

#ifndef __DEBUG_H__
#define __DEBUG_H__
#ifdef USE_WINDOWS

#include <stdlib.h>  // _MAX_PATH

#ifdef IS_DLL
#undef CDECL
#define CDECL __stdcall
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // added here for easiness
#define RED			"<FF0000>"
#define GREEN		"<158015>"
#define LIGHT_BLUE	"<0099CC>"
#define LIGHT_GREEN	"<10D510>"
#define VIOLET		"<DD0099>"
#define GRAY		"<808080>"
#define BROWN		"<B75727>"

    // static dimensions for the modules directory in shared memory
    // WARNING: if you touch these, you must touch them in DBGCTRL too!
    // static dimensions for the DLL's directory
#define DIR_ENTRIES 10
#define DIR_LENGTHS 32

    // static dimension for a single trace buffer
#define MAPSIZE 200*1024 - sizeof(struct modname)  // 200K per window

    // structure at the end of each shared memory

    struct modname {
        char name[DIR_LENGTHS];
        DWORD bias; // offset where to start putting characters
    }; // [DIR_ENTRIES] of them

    struct timing {
        LARGE_INTEGER Tbase, Tprev, Tcur, liDiff, liFreq;
    };

    // non-shared info for opened sharedmem's

    struct modentry {
        HANDLE hshmem, hmutex;
        LPVOID base;
        DWORD sot;
        BOOL newline; // TRUE if last printout was terminated with \n
        enum timing_mode t;
        struct timing mytime;
        char name[DIR_LENGTHS];
    };

    enum timing_mode {
        NO_TIMING,
        PARTIAL_TIMES,
        TOTAL_TIMES
    };

    extern char in_buf[]; // for interactivity
    extern BYTE debug;
    extern int g_shidx;

    extern wchar_t MyDir[_MAX_PATH];

    // prototypes used by this object
    int CDECL dbg_init(char *modname, enum timing_mode t);
    LPSTR CDECL dbg_version(HMODULE hMod); // added on 06jun01
    int CDECL dbg_input(char *prompt);
    LPSTR CDECL dbg_output(BYTE *buf, DWORD len); // format output on a returned string
    void CDECL dbg_printf(LPSTR format, ...); // output on sharedmem
    void CDECL dbg_trace(BYTE *buf, DWORD len, LPSTR hdrformat, ...);
    void CDECL dbg_error(LPSTR api, int error_type);
    void CDECL dbg_end(void);

    // to be used when one app wants to create more than one trace
    void CDECL dbg_printf_multi(int idx, LPSTR format, ...); // output on sharedmem
    void CDECL dbg_trace_multi(int idx, BYTE *buf, DWORD len, LPSTR hdrformat, ...);
    void dbg_error_multi(int shidx, LPSTR api, int error_type);
    void dbg_end_multi(int shidx);

    // used by DBGCTRL.EXE
    long map_get(int shidx, LPSTR mybuf, LPDWORD psz, BOOL from_sot);
    BOOL map_clean(int shidx);
    BOOL dir_get(void);

    void dbg_set_sot(int idx/*g_shidx*/); // set start-of-trace
    // get encrypted trace, ret len or -1
    int dbg_get_trc(int idx/*g_shidx*/, LPSTR *mybuf, BOOL from_sot);

    // conversion issues
    LPWSTR to_unicode(LPSTR s, LPWSTR d, unsigned u);
    LPSTR to_ascii(LPWSTR ws, LPSTR d, unsigned u);

    // all these have three args: src, dst, dstlen, and return dst,
    // but need an already allocated "dst" buffer
#ifdef UNICODE
#define ASCII_TO_GENERIC(s, d, dl) (to_unicode(s, d, dl), d)
#define UNICODE_TO_GENERIC(ws, d, dl) (wcscpy_s(d, dl, ws), d)
#define GENERIC_TO_ASCII(ws, d, dl) (to_ascii(ws, d, dl), d)
#define GENERIC_TO_UNICODE(ws, d, dl) (wcscpy_s(d, dl, ws), d)
#else
#define ASCII_TO_GENERIC(s, d, dl) (strcpy_s(d, dl, s), d)
#define UNICODE_TO_GENERIC(ws, d, dl) (to_ascii(ws, d, dl), d)
#define GENERIC_TO_ASCII(s, d, dl) (strcpy_s(d, dl, s), d)
#define GENERIC_TO_UNICODE(s, d, dl) (to_unicode(s, d, dl), d)
#endif


#if _MSC_VER==1200  // MSVC++ 6.0
#define strcpy_s(dst, dstlen, src) strcpy(dst, src)
#define wcscpy_s(dst, dstlen, src) wcscpy(dst, src)
#define strncpy_s(dst, dstlen, src, count) strncpy(dst, src, count)
#define strcat_s(dst, dstlen, src) strcat(dst, src)
#define vsprintf_s(buf, len, fmt, list) vsprintf(buf, fmt, list)
#define _wsplitpath_s(path, drive, drivelen, dir, dirlen, fname, fnamelen,\
	ext, extlen)\
	(_wsplitpath(path, drive, dir, fname, ext), 0)
#endif

    // useful with TOTAL_TIMES; use 0 as first argument
    void init_time(struct timing *t);

    LPSTR show_flags(UINT flg, char *legenda[]/*LSB first*/);

#ifdef __cplusplus
}
#endif

#endif
#endif//__DEBUG_H__
