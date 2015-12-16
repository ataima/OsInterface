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

#ifndef _IRAWDATA_HEADER
#define	_IRAWDATA_HEADER

class IRawData {
public:
    virtual size_t getSize() = 0;
    virtual int valid(void) = 0;
    virtual unsigned char *getData() = 0;
    virtual int toString(std::string & out, size_t start = 0, size_t size = 0) = 0;
    virtual unsigned char getAt(size_t npos) = 0;
    virtual unsigned char setAt(size_t npos, unsigned char data) = 0;

    virtual ~IRawData() {
    }

};




#endif	/* _IRAWDATA_H */

