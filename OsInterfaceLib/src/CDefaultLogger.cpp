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
#include <string.h>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <stdlib.h>
#include "../inc/IRawData.h"
#include "../inc/ILogger.h"
#include "../inc/ITime.h"
#include "../inc/IPort.h"
#include "../inc/IFile.h"
#include "../inc/IController.h"
#include "../inc/CGenericTime.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/CFile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <iomanip>


#define MAX_PRINTF  0x3fff

namespace logger {


#ifdef USE_WINDOWS

    int CExternLogger::Open(const char *filename) {
        int res = -1;
        int th = (int) ::GetCurrentThreadId();
        char buff[64];
        sprintf(buff, "Dbg:Th%d", th);
        mHandle = ::dbg_init(buff, NO_TIMING);
        if (mHandle != -1)
            res = 0;
        return res;
    }

    int CExternLogger::Close(void) {
        if (mHandle != -1)::dbg_end();
        mHandle = -1;
        return 0;
    }

    int CExternLogger::Write(const char *msg, unsigned int size) {
        dbg_printf("%s", msg);
        return 0; // force oK
    }


#endif

    CDefaultLogger::CDefaultLogger() {
        mHandle = -1;
    }

    CDefaultLogger::~CDefaultLogger() {
    }

    bool CDefaultLogger::enable() {
        return ( mHandle != -1);
    }

    void CDefaultLogger::printf(const char * Format, ...) {
        unsigned int size = 0;
        char pBuff[MAX_PRINTF];
        CGenericTime::getZeroTime()->getTime();
        size = sprintf(pBuff, "%s> ", CGenericTime::getZeroTime()->toString());
        Write(pBuff, size);
        va_list args;
        va_start(args, Format);
        size = vsnprintf(pBuff, MAX_PRINTF, Format, args);
        va_end(args);
        Write(pBuff, size);
    }

    void CDefaultLogger::dump(const char *msg, void * pbuffer, size_t size) {
        unsigned char * buffer = (unsigned char *) pbuffer;
        std::stringstream str;
        size_t i, u, x, nb, pos, width, addr;
        if (size > 256) {
            nb = size / 256;
            width = 256;
        } else {
            nb = 1;
            width = size;
        }
        str.str().reserve(8 * width);
        str << std::setfill('0');
        addr = 0;
        for (x = 0; x < nb; x++) {
            for (i = 0; i < width;) {
                pos = i;
                for (u = 0; u < 16 && i < width; u++, i++) {
                    str << "[" << std::setw(2) << std::hex << (short) (buffer[addr + i]) << "] ";
                }
                i = pos;
                str << " - ";
                for (u = 0; u < 16 && i < width; u++, i++) {
                    if (buffer[addr + i] < ' ') {
                        str << ".";
                    } else
                        if (buffer[addr + i] & 128) {
                        str << "^";
                    } else {
                        str << (char) (buffer[addr + i]);
                    }
                }
                str << std::endl;
            }
            this->printf("%s : OFF(%d):DIM(%d)\n%s", msg, addr,size, str.str().c_str());
            str.str("");
            addr += width;
        }
    }

    int CDefaultLogger::Open(const char *filename) {
        int res = 0;
        if (filename != NULL) {
            res = hardware::CFile::Open(mHandle, filename);
        }
        return res;
    }

    int CDefaultLogger::Close(void) {
        return hardware::CFile::Close(mHandle);
    }

    int CDefaultLogger::Write(const char *msg, unsigned int size) {
        size_t writed;
        return hardware::CPort::Write(mHandle, (unsigned char *) msg, size, writed, 0);
    }

    CMaxSizeLogger::CMaxSizeLogger(size_t max_size) {
        maxsize = max_size;
        curr_size = 0;
    }

    CMaxSizeLogger::~CMaxSizeLogger() {
        maxsize = 0;
        curr_size = 0;
    }
    // virtual for specific implementtion on derived classes

    int CMaxSizeLogger::Write(const char *msg, unsigned int size) {
        int res = CDefaultLogger::Write(msg, size);
        if (res > 0) {
            curr_size += res;
            if (curr_size > maxsize) {
                Close();
                curr_size = 0;
            }
        }
        return res;
    }

    CAutoMaxSizeLogger::CAutoMaxSizeLogger(size_t max_size, const char *path) : CMaxSizeLogger(max_size) {
        filename.clear();
        log_path = path;
        new_file = false;
    }

    CAutoMaxSizeLogger::~CAutoMaxSizeLogger() {
        log_path.clear();
        filename.clear();
        new_file = false;
    }

    int CAutoMaxSizeLogger::Open(const char *req_filename) {
        int res = -1;
        if (req_filename == NULL) {
            //autogenerate name
            std::stringstream logname;
            logname.str("");
            logname << log_path << "/";
            res = hardware::CFile::MkDir(logname.str().c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
            if (errno != EEXIST) {

            } else {
                CGenericTime t;
                CGenericTime::getCurrentTime()->getTime(&t);
                srand(t.getMilliSec());
                logname << rand() << "_" << t.getSecond() << t.getMilliSec() << "_log.txt";
                filename = logname.str();
                res = CMaxSizeLogger::Open(filename.c_str());
            }

        } else {
            res = CMaxSizeLogger::Open(req_filename);
        }
        return res;
    }

    int CAutoMaxSizeLogger::Write(const char *msg, unsigned int size) {
        int res = CMaxSizeLogger::Write(msg, size);
        if (res > 0) {
            if (mHandle == -1) {
                Open();
                new_file = true;
            }
        }
        return res;
    }

    CContAMSLogger::CContAMSLogger(size_t max_size, const char *path, const char * _history, IController * t) :
    CAutoMaxSizeLogger(max_size, path) {
        controller = t;
        history = _history;
    }

    CContAMSLogger::~CContAMSLogger() {
        controller = NULL;
    }

    int CContAMSLogger::Write(const char *msg, unsigned int size) {
        int res = CMaxSizeLogger::Write(msg, size);
        if (mHandle == -1) {
            controller->getParam(2, filename);
            Open(filename.c_str());
            std::stringstream s;
            s << log_path << "/" << history;
            new_file = false;
            if (controller != NULL) {
                res = controller->Start();
            }
            hardware::CFile f;
            if (f.Open(s.str().c_str(), O_APPEND | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO) == 0) {
                s.str("");
                size_t writed;
                s << filename << std::endl;
                f.Write((unsigned char *) (const_cast<char *> (s.str().c_str())), s.str().size(), writed, 0);
                f.Close();
            }
        }
        return res;
    }

    int CContAMSLogger::Open(const char *req_filename) {
        int res = -1;
        if (req_filename == NULL) {
            res = CAutoMaxSizeLogger::Open();
        } else {
            filename = req_filename;
            struct stat st;
            if (stat(req_filename, &st) == -1) {
                res = CMaxSizeLogger::Open(req_filename);
            } else {
                if ((req_filename == NULL) || (strcmp(req_filename, "stdout") == 0)) {
                    mHandle = 0; // stdout
                } else
                    if ((strcmp(req_filename, "stderr") == 0)) {
                    mHandle = 2; // stderr
                } else {
                    mHandle = open(req_filename, O_APPEND | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO);
                    if (mHandle < 0) {
                        res = -1;
                    } else {
                        curr_size = st.st_size;
                        this->printf(" RESTART NEW SESSION OF LOGGING\n");
                        this->printf(" ----------------------------------------------------------------\n\n\n");
                        res = 0;
                    }
                }

            }
        }
        return res;
    }


}


