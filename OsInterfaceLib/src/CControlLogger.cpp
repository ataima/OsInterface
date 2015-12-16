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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#ifdef USE_LINUX
#include <sys/stat.h>
#endif
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/CCircularBuffer.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/IController.h"
#include "../inc/CBaseController.h"
#include "../inc/CControlLogger.h"
#include "../inc/CMapToPointer.h"
#include "../inc/IPort.h"
#include "../inc/IFile.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/CFile.h"

#include "../inc/hardware_configure.h"


namespace utility {

    int CCircularLogFileNames::LoadFromHistory(std::string path, FILE *file) {
        char buff[1024];
        std::string str;
        int res = -1;
        if (file != NULL) {
            buff[0] = '\0';
            size_t path_size = path.size() + 1;
            while (feof(file) == 0) {
                memset(buff, 0, path_size);
                fgets(buff, 1024, file);
                //printf("CCircularLogFileNames::LoadFromHistory > HISTORY FILE :%s\n", buff);
                if (strlen(buff) > path_size) {
                    const char * p = &buff[path_size];
                    int v = atoi(p);
                    //printf("CCircularLogFileNames::LoadFromHistory > EXTRACT VALUE :%d\n", v);
                    this->Insert(v);
                }
                //printf("CCircularLogFileNames::LoadFromHistory > HISTORY FILE :%d files %d:%d\n", Size(), start, stop);
            }
            if (Size() != 0)
                res = 0;
        }
        return res;
    }

    CDirRules::CDirRules(const char * path, const char *_history, size_t max_size, size_t max_file) {
        if (_history != NULL) {
            history = _history;
        }
        if (path != NULL) {
            pathname = path;
        }
        max_path_size = max_size;
        max_num_file = max_file;
    }

    CDirRules::~CDirRules() {
        pathname.clear();
        max_path_size = 0;
        max_num_file = 0;
    }

    CControlLogger::CControlLogger() {
        mth_service = -1;
        mtx_files = -1;
    }

    CControlLogger::~CControlLogger() {
        mth_service = -1;
    }

    static void * handleCheckControlLogger(void *param) {
        CControlLogger * p = obj_cast<CControlLogger *>(param);
        return p->handlerRunControl(p);
    }

    void * CControlLogger::handlerRunControl(CControlLogger * p) {
        if (device == NULL)
            return NULL;
        software->MutexLock(mtx_files);
        /* this Controller check the dir of the pathname and verify if num file content in is minus of max num file
        and also  the total size occupied is minus of max path size*/
        if (cur_rule.pathname.size() > 0) {
            size_t total_size = 0;
            size_t total_num = 0;
            /* create the dir and compute the total size eq ls + du */
            dirInfo files;
            if (hardware::CFile::Dir(cur_rule.pathname.c_str(), files, total_size) == 0) {
                std::map<std::string, size_t>::iterator it, _end;
                total_num = files.size();
                it = files.find(cur_rule.history);
                if (it != files.end()) {
                    files.erase(it);
                    total_num--;
                }
                if ((total_size > cur_rule.max_path_size) ||
                        (total_num == (cur_rule.max_num_file - 1))) {
                    size_t th_size, th_file;
                    th_size = cur_rule.max_path_size - ((cur_rule.max_path_size * 2) / 10);
                    th_file = cur_rule.max_num_file - ((cur_rule.max_num_file * 2) / 10);
                    std::string all, str;
                    do {
                        getOldestFileName(str);
                        it = files.find(str);
                        if (it != files.end()) {
                            remove(str.c_str());
                            total_size -= it->second;
                            total_num--;
                            files.erase(it);
                        }
                    } while (((total_size > th_size) || (total_num > th_file)));
                    if (cur_index.Size() > 0) {
                        std::stringstream s;
                        size_t i, start, stop;
                        cur_index.getPosition(start, stop);
                        all.clear();
                        if (stop >= start) {
                            for (i = start; i <= stop; i++) {
                                s.str("");
                                s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << i << ".log";
                                it = files.find(s.str());
                                if (it != files.end()) {
                                    files.erase(it);
                                    all += s.str();
                                    all += '\n';
                                }
                            }
                        } else {
                            for (i = start; i < cur_rule.max_num_file; i++) {
                                s.str("");
                                s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << i << ".log";
                                it = files.find(s.str());
                                if (it != files.end()) {
                                    files.erase(it);
                                    all += s.str();
                                    all += '\n';
                                }
                            }
                            for (i = 1; i <= stop; i++) {
                                s.str("");
                                s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << i << ".log";
                                it = files.find(s.str());
                                if (it != files.end()) {
                                    files.erase(it);
                                    all += s.str();
                                    all += '\n';
                                }
                            }
                        }
                    }

                    if (files.size() > 0) {
                        // there are some files in dir not in history log
                        it = files.begin();
                        _end = files.end();
                        while (it != _end) {
                            remove(it->first.c_str());
                            it++;
                        }
                    }
                    FILE *file = fopen(cur_rule.history.c_str(), "w");
                    if (file != NULL) {
                        fwrite(all.c_str(), 1, all.size(), file);
                        fflush(file);
                        fclose(file);
                    }
                }
            }
        }
        software->MutexUnlock(mtx_files);
        int cur_mth = mth_service;
        mth_service = -1;
        if (VALID_HANDLE(cur_mth)) {

            software->DestroyThread(cur_mth);
        }
        return NULL;
    }

    int CControlLogger::CheckControl(IRuleList * rules) {
        Initialize();
        int res = control_logger_generic_error;
        if (device != NULL) {
            if (rules != NULL && rules->size() == 1) {
                CDirRules *rule = dynamic_cast<CDirRules *> (rules->front());
                if (rule != NULL) {
                    cur_rule = *rule;
                    if (software->CreateBinaryMutex(mtx_files, false) == 0) {
                        cur_index.SetWidth(cur_rule.max_num_file);
                        struct stat st;
                        if (stat(cur_rule.history.c_str(), &st) == -1) {
                            hardware::CFile file;
                            res = file.Open(cur_rule.history.c_str());
                            LOG(logger, "%s:%d : Create file for history %s\n", __FUNCTION__, __LINE__, cur_rule.history.c_str());
                        } else {
                            LOG(logger, "%s:%d : Open file for parse history %s\n", __FUNCTION__, __LINE__, cur_rule.history.c_str());
                            FILE *file = fopen(cur_rule.history.c_str(), "r+");
                            if (file != NULL) {
                                res = cur_index.LoadFromHistory(cur_rule.pathname, file);
                                fclose(file);
                            }
                        }
                        if (cur_index.Size() == 0) {
                            // No file in history
                            FILE *file = fopen(cur_rule.history.c_str(), "a");
                            if (file != NULL) {
                                std::string next;
                                getCurrentFileName(next);
                                next += '\n';
                                fwrite(next.c_str(), 1, next.size(), file);
                                fclose(file);
                            }
                        }

                    } else {
                        res = control_logger_create_mutex;
                    }
                } else {
                    res = control_logger_no_valid_rules;
                }
            } else {
                res = control_logger_no_valid_rules;
            }

        } else {

            res = control_logger_not_initialized;
        }
        return res;
    }

    int CControlLogger::Start(void) {
        int res = control_logger_generic_error;
        if (device != NULL) {
            if (software->CreateThread((void(*)())handleCheckControlLogger, this, mth_service) == 0) {
                res = 0;
            } else {
                res = control_logger_no_start_service;
            }
        } else {

            res = control_logger_not_initialized;
        }
        return res;
    }

    int CControlLogger::Close(void) {
        int res = control_logger_generic_error;
        if (device != NULL) {
            if (VALID_HANDLE(mth_service)) {
                res = software->DestroyThread(mth_service);
                mth_service = -1;
            }

            if (VALID_HANDLE(mtx_files)) {

                res = software->DestroyMutex(mtx_files);
            }

        }
        return res;
    }

    int CControlLogger::getNextFileName(std::string & out) {
        std::stringstream s;
        int v = cur_index.getNextIndexFile();
        cur_index.Insert(v);
        s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << v << ".log";
        out = s.str();
        //LOG(logger, "CControlLogger::getNextFileName : return  %s\n", out.c_str());

        return 0;
    }

    int CControlLogger::getOldestFileName(std::string & out) {
        std::stringstream s;
        int v = cur_index.getOldestIndexFile();
        cur_index.Remove();
        s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << v << ".log";
        out = s.str();
        //LOG(logger, "CControlLogger::getOldestFileName : return  %s\n", out.c_str());

        return 0;
    }

    int CControlLogger::getCurrentFileName(std::string & out) {
        std::stringstream s;
        int v = cur_index.getCurrentIndexFile();
        s << cur_rule.pathname << "/" << std::setw(4) << std::setfill('0') << v << ".log";
        out = s.str();
        //LOG(logger, "CControlLogger::getCurrentFileName : return  %s\n", out.c_str());

        return 0;
    }

    int CControlLogger::getParam(int off, std::string & out) {
        out.clear();
        switch (off) {
            case 0: return getOldestFileName(out);
                break;
            case 1: return getCurrentFileName(out);
                break;
            case 2: return getNextFileName(out);
                break;
        }
        return 0;
    }


}