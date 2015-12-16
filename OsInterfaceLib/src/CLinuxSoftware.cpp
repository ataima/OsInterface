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
#ifdef USE_LINUX
#include <stdio.h>
#include <string.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <errno.h>
#include "../inc/ISoftware.h"
#include "../inc/IMapToPointer.h"
#include "../inc/CMapToPointer.h"
#include <pthread.h>
#include <semaphore.h>
#include "../inc/CLinuxSoftware.h"
#include <sys/time.h>
#include "../inc/hardware_configure.h"



typedef void * (*functor)(void *);

namespace software {

    CSoftware::CSoftware() {
        int res = -1;
        pthread_mutexattr_t attr;
        mtx_obj_map = new pthread_mutex_t;
        if (mtx_obj_map != NULL) {
            if (pthread_mutexattr_init(&attr) == 0) {
                if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) == 0) {
                    if (pthread_mutex_init(mtx_obj_map, &attr) == 0) {
                        memset(global_mtx, -1, sizeof (global_mtx));
                        res = 0;
                    }
                }
            }
        }
        if (res != 0) {
            delete mtx_obj_map;
        }
    }

    CSoftware::~CSoftware() {
        size_t i;
        for (i = 0; i<sizeof (global_mtx); i++) {
            if (VALID_HANDLE(global_mtx[i])) {
                DestroyMutex(global_mtx[i]);
            }
        }
        if (mtx_obj_map != NULL) {
            pthread_mutex_destroy(mtx_obj_map);
            delete mtx_obj_map;
            mtx_obj_map = NULL;
        }


    }

    void CSoftware::LockMapObj(void) {
        if (mtx_obj_map != NULL)
            pthread_mutex_lock(mtx_obj_map);
    }

    void CSoftware::UnlockMapObj(void) {
        if (mtx_obj_map != NULL)
            pthread_mutex_unlock(mtx_obj_map);
    }

    int CSoftware::CreateThread(void (*entry)(), void * param, int & th_id) {
        int ret;
        pthread_attr_t tattr;
        pthread_t *thread_id;
        thread_id = new pthread_t();
        if (thread_id != NULL) {
            ret = pthread_attr_init(&tattr);
            if (ret == 0 || ret == EBUSY) {
                ret = pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);
                if (ret == 0) {
                    functor func = NULL;
                    func = (functor) (entry);
                    ret = pthread_create(thread_id, &tattr, func, param);
                    if (ret == 0) {
                        LockMapObj();
                        th_id = objects.AddObject(thread_id);
                        UnlockMapObj();
                    }
                }
            }
        }
        return ret;
    }

    int CSoftware::JoinThread(int & th_id) {
        int ret = -1;
        if (VALID_HANDLE(th_id)) {
            pthread_t *thread_id;
            LockMapObj();
            thread_id = utility::obj_cast<pthread_t *>(objects.PointerFromId(th_id));
            UnlockMapObj();
            ret = pthread_join(*thread_id, NULL);
            LockMapObj();
            objects.RemoveObject(th_id);
            UnlockMapObj();
            th_id = -1;
            delete thread_id;
        }
        return ret;
    }

    int CSoftware::DestroyThread(int & th_id) {
        int ret = -1;
        pthread_t *thread_id = NULL;
        if (VALID_HANDLE(th_id)) {
            LockMapObj();
            thread_id = utility::obj_cast<pthread_t *>(objects.PointerFromId(th_id));
            UnlockMapObj();
            if (thread_id != NULL) {
                ret = pthread_cancel(*thread_id);
                LockMapObj();
                objects.RemoveObject(th_id);
                UnlockMapObj();
                if (ret == ESRCH) // no thread found probability is already ended
                    ret = 0;
            }
            delete thread_id;
        }
        return ret;
    }

    int CSoftware::SleepThread(unsigned int delay) {
        if (delay < 1000)
            usleep(delay * 1000);
        else {
            usleep((delay % 1000)*1000);
            sleep(delay / 1000);
        }
        return 0;
    }

    unsigned int CSoftware::GetThreadId(void) {
        return (unsigned int) pthread_self();
    }

    int CSoftware::CreateBinaryMutex(int & mtx_id, bool locked) {
        int res = -1;
        pthread_mutexattr_t attr;
        pthread_mutex_t *m_tx = new pthread_mutex_t;
        if (m_tx != NULL) {
            if (pthread_mutexattr_init(&attr) == 0) {
                if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) == 0) {
                    if (pthread_mutex_init(m_tx, &attr) == 0) {
                        if (pthread_mutexattr_destroy(&attr) == 0) {
                            res = 0;
                            if (locked == true) {
                                res = pthread_mutex_lock(m_tx);
                            }
                            LockMapObj();
                            mtx_id = objects.AddObject(m_tx);
                            UnlockMapObj();
                        }
                    }
                }
            }
        }
        return res;
    }

    int CSoftware::DestroyMutex(int & mtx_id) {
        int ret = -1;
        if (VALID_HANDLE(mtx_id)) {
            pthread_mutex_t *m_tx = NULL;
            LockMapObj();
            m_tx = utility::obj_cast<pthread_mutex_t *>(objects.PointerFromId(mtx_id));
            UnlockMapObj();
            if (m_tx != NULL) {
                ret = pthread_mutex_destroy(m_tx);
                LockMapObj();
                objects.RemoveObject(mtx_id);
                UnlockMapObj();
                mtx_id = -1;
                delete m_tx;
            }
        }
        return ret;
    }

    int CSoftware::MutexLock(int mtx_id) {
        int ret = -1;
        if (VALID_HANDLE(mtx_id)) {
            pthread_mutex_t *m_tx = NULL;
            LockMapObj();
            m_tx = utility::obj_cast<pthread_mutex_t *>(objects.PointerFromId(mtx_id));
            UnlockMapObj();
            if (m_tx != NULL) {
                ret = pthread_mutex_lock(m_tx);
            } else {
                printf("ERROR : MutexLock :: m_tx =NULL for id=%d \n", mtx_id);
            }
        }
        return ret;
    }

    int CSoftware::MutexUnlock(int mtx_id) {
        int ret = -1;
        if (VALID_HANDLE(mtx_id)) {
            pthread_mutex_t *m_tx = NULL;
            LockMapObj();
            m_tx = utility::obj_cast<pthread_mutex_t *>(objects.PointerFromId(mtx_id));
            UnlockMapObj();
            if (m_tx != NULL) {
                ret = pthread_mutex_unlock(m_tx);
            }
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
            if (!VALID_HANDLE(global_mtx[offset])) {
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



    // Semaphore : warnin inter thread no IPC

    int CSoftware::CreateSemph(int & mph_id, int vinit) {
        int res = -1;
        sem_t *m_p = new sem_t;
        if (m_p != NULL) {
            if (sem_init(m_p, 0, vinit) == 0) {
                res = 0;
                LockMapObj();
                mph_id = objects.AddObject(m_p);
                UnlockMapObj();
            }
        }
        return res;
    }

    int CSoftware::DestroySemph(int & mph_id) {
        int ret = -1;
        if (VALID_HANDLE(mph_id)) {

            sem_t *m_p = NULL;
            LockMapObj();
            m_p = utility::obj_cast<sem_t *>(objects.PointerFromId(mph_id));
            UnlockMapObj();
            if (m_p != NULL) {
                ret = sem_destroy(m_p);
                LockMapObj();
                objects.RemoveObject(mph_id);
                UnlockMapObj();
                mph_id = -1;
                delete m_p;
            }
        }
        return ret;
    }

    int CSoftware::SemphWait(int mph_id) {
        int ret = -1;
        if (VALID_HANDLE(mph_id)) {
            sem_t *m_p = NULL;
            LockMapObj();
            m_p = utility::obj_cast<sem_t *>(objects.PointerFromId(mph_id));
            UnlockMapObj();
            if (m_p != NULL) {
                ret = sem_wait(m_p);
            }
        }
        return ret;
    }

    int CSoftware::SemphTimeWait(int mph_id, unsigned int time) {
        int ret = -1;
        if (VALID_HANDLE(mph_id)) {
            sem_t *m_p = NULL;
            LockMapObj();
            m_p = utility::obj_cast<sem_t *>(objects.PointerFromId(mph_id));
            UnlockMapObj();
            if (m_p != NULL) {
                struct timeval now;
                struct timeval req;
                struct timeval result;

                req.tv_sec = time / 1000;
                req.tv_usec = (time % 1000)*1000;

                gettimeofday(&now, NULL);
                timeradd(&now, &req, &result);
                struct timespec end;
                end.tv_sec = result.tv_sec;
                end.tv_nsec = result.tv_usec * 1000;
                do {
                    errno = 0;
#ifdef __MACH__                     
                    ret = sem_wait(m_p);
#else
                    ret = sem_timedwait(m_p, &end);
#endif                    
                } while (errno == EINTR);
            }
        }
        return ret;
    }

    int CSoftware::SemphPost(int mph_id) {
        int ret = -1;
        if (VALID_HANDLE(mph_id)) {
            sem_t *m_p = NULL;
            LockMapObj();
            m_p = utility::obj_cast<sem_t *>(objects.PointerFromId(mph_id));
            UnlockMapObj();
            if (m_p != NULL) {
                ret = sem_post(m_p);
            }
        }
        return ret;
    }
}

#endif
