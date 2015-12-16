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

#ifndef _CCIRCULARBUFFER_H
#define	_CCIRCULARBUFFER_H

//simple class for thread rx
#include <vector>

namespace utility {

    template <typename T>
    class CCircularBuffer
    : public std::vector< T > {
        size_t WIDTH;
        size_t start;
        size_t stop;
    public:

        CCircularBuffer(size_t assigned_width = 128) {
            WIDTH = assigned_width;
            this->resize(WIDTH, 0);
            start = stop = 0;
        }

        inline bool Empthy(void) {
            return start == stop;
        }

        bool Insert(T *ptr, size_t width) {
            bool res = false;
            //    if(start!=stop || stop==0)
            //    {
            size_t i;
            size_t p;
            for (i = 0; i < width; i++) {
                p = (stop + i) % WIDTH;
                if (Empthy() == false && p == start) {
                    i--;
                    break;
                }
                T v = ptr[i];
                this->at(p) = v;
            }
            if (i == width)
                res = true;
            stop = (stop + i) % WIDTH;
            //    }
            return res;
        }

        bool Remove(T *ptr, size_t width) {
            bool res = false;
            if (start != stop) {
                size_t i;
                size_t p;
                for (i = 0; i < width; i++) {
                    p = (start + i) % WIDTH;
                    if (Empthy() == false && p == stop) {
                        i--;
                        break;
                    }
                    T v = this->at(p);
                    ptr[i] = v;
                }
                if (i == width)
                    res = true;
                start = (start + i) % WIDTH;
            }
            return res;
        }

        size_t Size(void) {
            if (stop >= start)
                return (stop - start);
            else
                return (WIDTH - start + stop);
        }

        void Clean(void) {
            size_t i;
            for (i = 0; i < WIDTH; i++) {
                this->at(i) = 0;
            }
            start = stop = 0;
        }

    };

}

#endif	/* _CCIRCULARBUFFER_H */

