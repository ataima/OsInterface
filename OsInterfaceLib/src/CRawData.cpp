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
#include <time.h>
#include <string>
#include <sstream>

#include "../inc/IRawData.h"
#include "../inc/CRawData.h"


namespace utility {

    CRawData::CRawData() {
        data = NULL;
        size = 0;
    }

    CRawData::~CRawData() {
        if (data != NULL) {
            delete[] data;
            data = NULL;
        }
        size = 0;
    }

    void CRawData::operator=(CRawData & b) {
        memcpy(data, b.getData(), b.getSize());
        size = b.getSize();
    }

    int CRawData::toString(std::string & out, size_t start, size_t wsize) {
        std::stringstream str;
        size_t i, u, width, pos;
        if (wsize == 0) {
            width = size;
        } else {
            width = (start + wsize);
            if (width > size)
                width = size;
        }
        for (i = start; i < width;) {
            pos = i;
            for (u = 0; u < 16 && i < width; u++, i++) {
                str << "[" << std::hex << (short) (data[i]) << "] ";
            }
            i = pos;
            str << " - ";
            for (u = 0; u < 16 && i < width; u++, i++) {
                if (data[i] < ' ') {
                    str << ".";
                } else {
                    str << (char) (data[i]);
                }
            }
            str << std::endl;
        }
        out = str.str();
        return (int) (str.str().size());
    }

    unsigned char CRawData::getAt(size_t npos) {
        if (npos < size) {
            return data[npos];
        } else
            throw ("CRawData::GetAt()::Out of range exception!");
    }

    unsigned char CRawData::setAt(size_t npos, unsigned char value) {
        if (npos < size) {
            data[npos] = value;
        } else
            throw ("CRawData::SetAt()::Out of range exception!");
        return value;
    }

    unsigned char CRawData::setAt(unsigned char *d, size_t s) {
        unsigned char res = -1;
        if (s > 0 && d != NULL) {
            data = new unsigned char [s];
            if (data != NULL) {
                size = s;
                memcpy(data, d, size);
                res = 0;
            }
        }
        return res;
    }

    /*! return !=0 if data is a valid pointer and size > 0*/
    int CRawData::valid(void) {
        return (data != NULL && size > 0);
    }

}


