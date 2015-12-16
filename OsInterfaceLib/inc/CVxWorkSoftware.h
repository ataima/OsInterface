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

#ifndef _CVXWORKTHREAD_H
#define	_CVXWORKTHREAD_H

#ifdef USE_VXWORK


namespace software {
    namespace vxwork {

        class CSoftware
        : public ISoftware {
        public:
            int CreateThread(void *entry, void * param, int & th_id);
            int JoinThread(int & th_id);
            int DestroyThread(int & th_id);
            int SleepThread(unsigned int delay);
            //
            int CreateMutex(int & mtx_id, bool locked = false);
            int DestroyMutex(int & mtx_id);
            int MutexLock(int mtx_id);
            int MutexUnlock(int mtx_id);
            // Semaphore : warnin inter thread no IPC
            int CreateSemph(int & mtx_id, int vinit = 0);
            int DestroySemph(int & mtx_id);
            int SemphWait(int mtx_id);
            int SemphTimeWait(int mtx_id, unsigned int time);
            int SemphPost(int mtx_id);

        };

    }

}

#endif
#endif	/* _CVXWORKTHREAD_H */

