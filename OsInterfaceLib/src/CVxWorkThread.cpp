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
#ifdef USE_VXWORK
#include <stdio.h>
#include <string.h>
#include "../inc/ISoftware.h"
#include "../inc/CVxWorkSoftware.h"



namespace software {
    namespace vxwork {

        int CSoftware::CreateThread(void *entry, void * param, int & th_id) {
            return 0;
        }

        int CSoftware::JoinThread(int & th_id) {
            return 0;
        }

        int CSoftware::DestroyThread(int & th_id) {
            return 0;
        }

        int CSoftware::SleepThread(unsigned int delay) {
            return 0;
        }

        int CSoftware::CreateMutex(int & mtx_id, bool locked) {
            return 0;
        }

        int CSoftware::DestroyMutex(int & mtx_id) {
            return 0;
        }

        int CSoftware::MutexLock(int mtx_id) {
            return 0;
        }

        int CSoftware::MutexUnlock(int mtx_id) {
            return 0;
        }

    }

}

#endif
