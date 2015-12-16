////////////////////////////////////////////////////////////////////////////////
//    Copyright (C) 2011  Angelo Coppi (coppi dot angelo at virgilio dot it )
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
// Author : Angelo Coppi (coppi dot angelo at virgilio dot it )
// History:        
// Created on 12 Nov 2011
////////////////////////////////////////////////////////////////////////////////
#ifdef USE_WINDOWS
#include <stdio.h>
#include <string.h>
#include <string>


#include <io.h>
#include <winsock.h>
#include <windows.h>



#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sstream>
#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/CFile.h"

static void ErrorReport(LPSTR lpszFunction) {
    // Retrieve the system error message for the last-error code

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR) & lpMsgBuf,
            0, NULL);

    // Display the error message and exit the process

    lpDisplayBuf = (LPVOID) LocalAlloc(LMEM_ZEROINIT,
            (strlen((LPCSTR) lpMsgBuf) + strlen((LPCSTR) lpszFunction) + 40) * sizeof (CHAR));
    sprintf((char *) lpDisplayBuf, "%s failed with error %d: %s",
            lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCSTR) lpDisplayBuf, "Error", MB_OK);
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


namespace hardware {

    CFile::CFile() {
        mHandle = -1;
    }

    CFile::~CFile() {
        if (mHandle > 2) {
            Close();
        }
    }

    int CFile::Open(int & handle, const char *name, int mode, int permission) {
        handle = -1;
        int res = ifile_generic_error;
        if (strcmp(name, "stdout") == 0) {
            handle = (int) GetStdHandle(STD_OUTPUT_HANDLE);
            res = 0;
        } else
            if ((strcmp(name, "stdin") == 0)) {
            handle = (int) GetStdHandle(STD_INPUT_HANDLE);
            res = 0;
        } else
            if ((strcmp(name, "stderr") == 0)) {
            handle = (int) GetStdHandle(STD_ERROR_HANDLE);
            res = 0;
        } else {
            if (mode == -1) {
                mode = O_CREAT | O_WRONLY | O_TRUNC;
            }
            if (permission == -1) {
                permission = S_IRWXU | S_IRWXG | S_IRWXO;
            }
            if ((mode & O_CREAT) == 0) {
                OFSTRUCT ofs;
                mode = OPEN_EXISTING;
                permission = FILE_ATTRIBUTE_NORMAL;
                handle = (int) OpenFile(name, &ofs, mode);
            } else {
                mode = CREATE_ALWAYS;
                permission = FILE_ATTRIBUTE_NORMAL;
                handle = (int) CreateFile(name, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, mode, permission, NULL);
            }
            //mHandle = _open(name, mode, permission);
            if (handle == HFILE_ERROR) {
                res = IFile::ifile_cannot_open;
                ErrorReport("Open");
            } else {
                res = 0;
            }
        }
        return res;
    }

    int CFile::Close(int & handle) {
        if (handle == (int) GetStdHandle(STD_OUTPUT_HANDLE)) {
        } else
            if (handle == (int) GetStdHandle(STD_INPUT_HANDLE)) {
        } else
            if (handle == (int) GetStdHandle(STD_ERROR_HANDLE)) {
        } else {
            CloseHandle((HANDLE) handle);
        }
        handle = -1;
        return 0;

    }

    int CFile::Open(const char *name, int mode, int permission) {
        int res = CFile::Open(mHandle, name, mode, permission);
        if (res == 0) {
            filename = name;
        }
        return res;
    }

    int CFile::Close(void) {
        int res = CFile::Close(mHandle);
        if (res == 0) {
            filename.clear();
        }
        return res;
    }

    // remove the file 

    int CFile::Remove(void) {
        int res = ifile_generic_error;
        if (isOpen() == true && filename[0] != '\0') {
            res = Close();
            if (res == 0) {
                res = remove(filename.c_str());
            }
        }
        return res;
    }

    int CFile::MkDir(const char *name, size_t permission) {
        return _mkdir(name);
    }

    int CFile::Dir(const char *pathname, dirInfo & files, size_t & total_size) {
        WIN32_FIND_DATA ffd;
        HANDLE hFind;
        int res = ifile_generic_error;
        hFind = FindFirstFile(pathname, &ffd);
        if (INVALID_HANDLE_VALUE == hFind) {
            res = ifile_cannot_open_dir;
        } else {

            do {
                std::stringstream fullname;
                fullname << pathname << "/" << ffd.cFileName;
                total_size += ffd.nFileSizeLow;
                files[fullname.str().c_str()] = ffd.nFileSizeLow;
            } while (FindNextFile(hFind, &ffd) != 0);
            FindClose(hFind);
            res = 0;
        }
        return res;
    }
}
#endif
