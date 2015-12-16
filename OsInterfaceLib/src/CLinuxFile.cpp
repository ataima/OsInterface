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
#include <string>





#include <sys/types.h>
#include <sys/stat.h>

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sstream>
#include <errno.h>

#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/IFile.h"
#include "../inc/CFile.h"
namespace hardware {

    CFile::CFile() {

    }

    CFile::~CFile() {
        if (mHandle > 2) {
            close(mHandle);
            mHandle = -1;
        }
    }

    int CFile::Open(int &handle, const char *name, int mode, int permission) {
        int res = ifile_generic_error;
        if (strcmp(name, "stdout") == 0) {
            handle = 0;
	    res=0;
        } else
            if ((strcmp(name, "stdin") == 0)) {
            handle = 1;
	    res=0;
        } else
            if ((strcmp(name, "stderr") == 0)) {
            handle = 2;
	    res=0;
        } else {
            if (mode == -1) {
                mode = O_CREAT | O_RDWR | O_TRUNC;
            }
            if (permission == -1) {
                permission = S_IRWXU | S_IRWXG | S_IRWXO;
            }
            handle = open(name, mode, permission);
            if (handle < 0) {
                res = ifile_cannot_open;
            } else {
                res = 0;
            }
        }
        return res;
    }

    int CFile::Open(const char *name, int mode, int permission) {
        int res = CFile::Open(mHandle, name, mode, permission);
        if (res == 0) {
            filename = name;
        }
        return res;
    }

    int CFile::Close(int & handle) {
        if (handle > 2) {
            close(handle);
        }
        handle = -1;
        return 0;
    }

    int CFile::Close(void) {
        int res = CFile::Close(mHandle);
        if (res == 0) {
            filename.clear();
        }
        return res;
    }



    // remove the file 

    int CFile::Remove(void) {
        int res = ifile_generic_error;
        if (isOpen() == true && filename[0] != '\0') {
            res = Close();
            if (res == 0) {
                res = remove(filename.c_str());
            }
        }
        return res;
    }

    int CFile::MkDir(const char *name, size_t permission) {
        return mkdir(name, permission);
    }

    int CFile::Dir(const char *pathname, dirInfo & files, size_t & total_size) {
        int res = ifile_generic_error;
        DIR *d;
        struct dirent *dir;
        files.clear();
        d = opendir(pathname);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                struct stat s;
                std::stringstream fullname;
                if (strcmp(dir->d_name, ".") == 0)
                    continue;
                if (strcmp(dir->d_name, "..") == 0)
                    continue;
                fullname << pathname << "/" << dir->d_name;
                if (stat(fullname.str().c_str(), &s) == 0) {
                    total_size += s.st_size;
                    files[fullname.str().c_str()] = s.st_size;
                    //printf("DIR FILES %s : %d\n", fullname.str().c_str(), (int) s.st_size);
                }
                res = 0;
            }
            closedir(d);
        } else {
            res = ifile_cannot_open_dir;
        }
        return res;
    }

}
#endif

