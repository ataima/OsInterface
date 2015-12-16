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

#ifndef IFILE_H
#define	IFILE_H

class IFile {
public:

    typedef enum tag_ifile_error {
        ifile_generic_error = -1,
        ifile_cannot_open = -10000,
        ifile_write_error,
        ifile_partial_write_error,
        ifile_read_timeout_errror,
        ifile_flush_error,
        ifile_cannot_open_dir
    } ifileError;
public:
    // Open the specified file
    virtual int Open(const char *name, int mode = -1, int permission = -1) = 0;
    // Close the specified file
    virtual int Close(void) = 0;
    // return file name    
    virtual const char * GetFilename(void) = 0;
    // remove the file 
    virtual int Remove(void) = 0;
};


#endif	/* IFILE_H */

