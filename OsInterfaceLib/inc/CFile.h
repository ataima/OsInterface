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
// Created on 13 Nov 2011
////////////////////////////////////////////////////////////////////////////////
#ifndef C_OSINTERFACE_FILE_H
#define	C_OSINTERFACE_FILE_H

#include "IFile.h"


#ifdef USE_LINUX
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h> 
#endif


#ifdef USE_WINDOWS
#include <io.h>
#include <winsock.h>
#include <direct.h>
#include <sys/stat.h>

#define  S_IRUSR _S_IREAD
#define  S_IRGRP _S_IREAD
#define  S_IROTH _S_IREAD

#define  S_IWUSR _S_IWRITE
#define  S_IWGRP _S_IWRITE
#define  S_IWOTH _S_IWRITE

#define  S_IRWXU _S_IREAD|_S_IWRITE 
#define  S_IRWXG _S_IREAD|_S_IWRITE 
#define  S_IRWXO _S_IREAD|_S_IWRITE 

#define write  _write
#define read   _read
#define open   _open
#define close  _close
#endif

#include <map> 




typedef std::map<std::string, size_t> dirInfo;

namespace hardware {

    class CFile
    : public IFile
    , public hardware::CPort {
        std::string filename;
    public:

        CFile();

        virtual ~CFile();

        // Open the specified file
        int Open(const char *name, int mode = -1, int permission = -1);

        // Close the specified file
        int Close(void);

        // return file name    

        inline const char * GetFilename(void) {
            return filename.c_str();
        }

        // remove the file 
        int Remove(void);

        static int MkDir(const char *name, size_t permission);

        static int Dir(const char *pathname, dirInfo & files, size_t & total_size);

        static int Open(int & handle, const char *name, int mode = -1, int permission = -1);

        static int Close(int & handle);

    };
}
#endif	

