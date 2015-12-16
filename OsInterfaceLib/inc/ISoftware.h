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

#ifndef _ISOFTWARE_H
#define	_ISOFTWARE_H


#define MAX_GLOBAL_OBJECT    1024

class ISoftware {
public:
    // Thread
    virtual int CreateThread(void (*entry)(), void * param, int & th_id) = 0;
    virtual int JoinThread(int & th_id) = 0;
    virtual int DestroyThread(int & th_id) = 0;
    virtual int SleepThread(unsigned int delay) = 0;
    virtual unsigned int GetThreadId(void) = 0;
    // Mutex
    virtual int CreateBinaryMutex(int & mtx_id, bool locked = false) = 0;
    virtual int DestroyMutex(int & mtx_id) = 0;
    virtual int MutexLock(int mtx_id) = 0;
    virtual int MutexUnlock(int mtx_id) = 0;
    virtual int CreateGlobalMutex(int offset) = 0;
    virtual int DestroyGlobalMutex(int offset) = 0;
    virtual int GlobalMutexLock(int offset) = 0;
    virtual int GlobalMutexUnlock(int offset) = 0;
    // Semaphore
    virtual int CreateSemph(int & mph_id, int vinit = 0) = 0;
    virtual int DestroySemph(int & mph_id) = 0;
    virtual int SemphWait(int mph_id) = 0;
    virtual int SemphTimeWait(int mph_id, unsigned int time) = 0;
    virtual int SemphPost(int mph_id) = 0;

    virtual ~ISoftware() {
    }

};

#endif	/* _ITHREAD_H */

