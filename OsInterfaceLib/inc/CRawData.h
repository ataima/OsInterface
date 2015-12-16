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

#ifndef _CRAWDATA_HEADER
#define	_CRAWDATA_HEADER

#include "IRawData.h"


namespace utility {

    class CRawData
    : public IRawData {
    protected:
        unsigned char *data;
        size_t size;
    public:
        CRawData();

        virtual ~CRawData();

        void operator=(CRawData & b);

        inline size_t getSize() {
            return size;
        }

        inline unsigned char *getData() {
            return data;
        }

        int valid(void);

        int toString(std::string & out, size_t start = 0, size_t wsize = 0);

        unsigned char getAt(size_t npos);

        unsigned char setAt(size_t npos, unsigned char data);

        unsigned char setAt(unsigned char *d = NULL, size_t s = 0);

    };


}


#endif	/* _CRAWDATA_H */

