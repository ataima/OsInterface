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

#ifndef _CWINDOWSTHREAD_H
#define	_CWINDOWSTHREAD_H




#include <windows.h>

namespace software {

    class CSoftware
    : public ISoftware {
        static LARGE_INTEGER userTime;
        utility::CMapToPointer objects;
        HANDLE mtx_obj_map;
        int global_mtx[MAX_GLOBAL_OBJECT];
    private:
        void LockMapObj(void);
        void UnlockMapObj(void);
        static LARGE_INTEGER CSoftware::Start(void);
        unsigned int GetSystemTime();
    public:
        CSoftware();
        virtual ~CSoftware();
        int CreateThread(void(*entry)(), void * param, int & th_id);
        int JoinThread(int & th_id);
        int DestroyThread(int & th_id);
        int SleepThread(unsigned int delay);
        unsigned int GetThreadId(void);
        //
        int CreateBinaryMutex(int & mtx_id, bool locked = false);
        int DestroyMutex(int & mtx_id);
        int MutexLock(int mtx_id);
        int MutexUnlock(int mtx_id);
        int CreateGlobalMutex(int offset);
        int DestroyGlobalMutex(int offset);
        int GlobalMutexLock(int offset);
        int GlobalMutexUnlock(int offset);

        // Semaphore : warnin inter thread no IPC
        int CreateSemph(int & mtx_id, int vinit = 0);
        int DestroySemph(int & mtx_id);
        int SemphWait(int mtx_id);
        int SemphTimeWait(int mtx_id, unsigned int time);
        int SemphPost(int mtx_id);

    };

}

#endif
#endif	/* _CWINDOWSTHREAD_H */

