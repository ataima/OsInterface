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

#ifndef _CDEFAULTLOGGER_H
#define	_CDEFAULTLOGGER_H

class IController;


#ifdef USE_WINDOWS
#include "windows.h"
#include "../inc/debug.h"
#endif


namespace logger {

    class CDefaultLogger
    : public ILogger {
    protected:
        int mHandle;
    public:
        CDefaultLogger();
        virtual ~CDefaultLogger();
        // virtual for specific implementtion on derived classes
        virtual int Open(const char *filename = NULL);
        virtual int Close(void);
        virtual int Write(const char *msg, unsigned int size);
        virtual void printf(const char * Format, ...);
        virtual void dump(const char *msg, void * buffer, size_t size);
        inline bool enable(void);
    };

#ifdef USE_WINDOWS

    class CExternLogger
    : public CDefaultLogger {
    public:
        virtual int Open(const char *filename = NULL);
        virtual int Close(void);
        virtual int Write(const char *msg, unsigned int size);
    };
#endif

    class CMaxSizeLogger
    : public CDefaultLogger {
    protected:
        size_t curr_size;
        size_t maxsize;
    public:
        CMaxSizeLogger(size_t max_size);
        virtual ~CMaxSizeLogger();
        // virtual for specific implementtion on derived classes
        virtual int Write(const char *msg, unsigned int size);
    };

    class CAutoMaxSizeLogger
    : public CMaxSizeLogger {
    protected:
        std::string log_path;
        std::string filename;
        bool new_file;
    public:
        CAutoMaxSizeLogger(size_t max_size, const char *path);
        virtual ~CAutoMaxSizeLogger();
        virtual int Open(const char *filename = NULL);
        virtual int Write(const char *msg, unsigned int size);

        inline const char *getFileName(void) {
            return filename.c_str();
        }
    };

    class CContAMSLogger
    : public CAutoMaxSizeLogger {
        IController * controller;
        std::string history;
    public:
        CContAMSLogger(size_t max_size, const char *path, const char *_history, IController * t);
        virtual ~CContAMSLogger();
        virtual int Write(const char *msg, unsigned int size);
        virtual int Open(const char *req_filename);
    };

}
#endif	/* _CDEFAULTLOGGER_H */

