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
#include <map>
#include "../inc/ISoftware.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CMapToPointer.h"
#include "../inc/CWindowsSoftware.h"
#include "../inc/hardware_configure.h"


typedef DWORD(_stdcall *functor)(void *);


namespace software {

    LARGE_INTEGER CSoftware::userTime = Start();

    LARGE_INTEGER CSoftware::Start(void) {
        QueryPerformanceCounter(&userTime);
        return userTime;
    }

    CSoftware::CSoftware() {
        mtx_obj_map = ::CreateMutex(NULL, FALSE, NULL);
    }

    CSoftware::~CSoftware() {
        if (mtx_obj_map != INVALID_HANDLE_VALUE) {
            CloseHandle(mtx_obj_map);
        }
    }

    void
    CSoftware::LockMapObj(void) {
        if (mtx_obj_map != INVALID_HANDLE_VALUE)
            WaitForSingleObject(mtx_obj_map, INFINITE);
    }

    void
    CSoftware::UnlockMapObj(void) {
        if (mtx_obj_map != INVALID_HANDLE_VALUE)
            ReleaseMutex(mtx_obj_map);
    }

    int CSoftware::CreateThread(void(*entry)(), void * param, int & th_id) {
        int res = -1;
        functor func = NULL;
        func = (functor) (entry);
        DWORD id = -1;
        HANDLE handle = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) func, param, 0, &id);
        if (handle != INVALID_HANDLE_VALUE) {
            LockMapObj();
            th_id = objects.AddObject(handle);
            UnlockMapObj();
            res = 0;
        }
        return res;
    }

    int CSoftware::JoinThread(int & th_id) {
        int ret;
        HANDLE handle;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(th_id));
        UnlockMapObj();
        DWORD exitCode = 0;
        int c = 0;
        while (c < 100) {
            GetExitCodeThread(handle, &exitCode);
            if (exitCode == STILL_ACTIVE) {
                Sleep(100);
                continue;
            }
            break;
            c++;
        }
        ret = (c > 99);
        CloseHandle(handle);
        LockMapObj();
        objects.RemoveObject(th_id);
        UnlockMapObj();
        th_id = -1;
        return ret;
    }

    int CSoftware::DestroyThread(int & th_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(th_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            if (!TerminateThread(handle, 0) == 0) {
                ResumeThread(handle);
                TerminateThread(handle, 0);
            }
            CloseHandle(handle);
            LockMapObj();
            objects.RemoveObject(th_id);
            UnlockMapObj();
            ret = 0;
        }
        return ret;
    }

    int CSoftware::SleepThread(unsigned int delay) {
        ::Sleep(delay);
        return 0;
    }

    unsigned int CSoftware::GetThreadId(void) {
        return (unsigned int) GetCurrentThreadId();
    }

    int CSoftware::CreateBinaryMutex(int & mtx_id, bool locked) {
        int res = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        if (locked == true) {
            handle = ::CreateMutex(NULL, TRUE, NULL);
        } else {
            handle = ::CreateMutex(NULL, FALSE, NULL);
        }
        if (handle != INVALID_HANDLE_VALUE) {
            LockMapObj();
            mtx_id = objects.AddObject(handle);
            UnlockMapObj();
            res = 0;
        }
        return res;
    }

    int CSoftware::DestroyMutex(int & mtx_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mtx_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            CloseHandle(handle);
            LockMapObj();
            objects.RemoveObject(mtx_id);
            UnlockMapObj();
            mtx_id = -1;
            ret = 0;
        }
        return ret;
    }

    int CSoftware::MutexLock(int mtx_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mtx_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ret = WaitForSingleObject(handle, INFINITE);
        }
        return ret;
    }

    int CSoftware::MutexUnlock(int mtx_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mtx_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ret = ReleaseMutex(handle) == 0;
        }
        return ret;

    }

    int CSoftware::CreateGlobalMutex(int offset) {
        int res = -1;
        if (offset < MAX_GLOBAL_OBJECT) {
            if (VALID_HANDLE(global_mtx[offset])) {
                res = 0;
            } else {
                res = CreateBinaryMutex(global_mtx[offset]);
            }
        }
        return res;
    }

    int CSoftware::DestroyGlobalMutex(int offset) {
        int res = -1;
        if (offset < MAX_GLOBAL_OBJECT) {
            if (VALID_HANDLE(global_mtx[offset]))
                res = DestroyMutex(global_mtx[offset]);
        }
        return res;
    }

    int CSoftware::GlobalMutexLock(int offset) {
        int res = -1;
        if (offset < MAX_GLOBAL_OBJECT) {
            if (!(VALID_HANDLE(global_mtx[offset]))) {
                res = CreateGlobalMutex(offset);
            }
            if (VALID_HANDLE(global_mtx[offset]))
                res = MutexLock(global_mtx[offset]);
        }
        return res;
    }

    int CSoftware::GlobalMutexUnlock(int offset) {
        int res = -1;
        if (offset < MAX_GLOBAL_OBJECT) {
            if (VALID_HANDLE(global_mtx[offset]))
                res = MutexUnlock(global_mtx[offset]);
        }
        return res;
    }

    int
    CSoftware::CreateSemph(int & mph_id, int vinit) {
        int res = -1;
        HANDLE handle = ::CreateSemaphore(NULL, vinit, 0x7fffffff, NULL);
        if (handle != INVALID_HANDLE_VALUE) {
            res = 0;
            LockMapObj();
            mph_id = objects.AddObject(handle);
            UnlockMapObj();
        }
        return res;
    }

    int
    CSoftware::DestroySemph(int & mph_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mph_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ::CloseHandle(handle);
            LockMapObj();
            objects.RemoveObject(mph_id);
            UnlockMapObj();
            mph_id = -1;
            ret = 0;
        }
        return ret;
    }

    int
    CSoftware::SemphWait(int mph_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mph_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ret = WaitForSingleObject(handle, INFINITE);
        }
        return ret;
    }

    int
    CSoftware::SemphTimeWait(int mph_id, unsigned int time) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mph_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ret = WaitForSingleObject(handle, time);
        }
        return ret;
    }

    int
    CSoftware::SemphPost(int mph_id) {
        int ret = -1;
        HANDLE handle = INVALID_HANDLE_VALUE;
        LockMapObj();
        handle = utility::obj_cast<HANDLE > (objects.PointerFromId(mph_id));
        UnlockMapObj();
        if (handle != INVALID_HANDLE_VALUE) {
            ret = ReleaseSemaphore(handle, 1, NULL) == 0;
        }
        return ret;
    }

    unsigned int CSoftware::GetSystemTime() {
        LARGE_INTEGER v, f;
        QueryPerformanceFrequency(&f);
        f.QuadPart /= 1000; //ms
        QueryPerformanceCounter(&v);
        v.QuadPart -= userTime.QuadPart;
        if (v.QuadPart < 0) {
            v.QuadPart = -v.QuadPart;
        }
        return static_cast<unsigned int> (v.QuadPart / f.QuadPart);
    }


}

#endif

