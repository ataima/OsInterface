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

#ifndef _CMAPTOPOINTER_H
#define	_CMAPTOPOINTER_H


namespace utility {

    typedef std::map<void *, int> ptrToId;
    typedef std::map<int, void *> idToPtr;

    class CMapToPointer {
    protected:
        ptrToId pmap;
        idToPtr imap;
        static long int UID_HANDLE;
    public:
        void *PointerFromId(int id);
        int IdFromPointer(void *);
        int AddObject(void *);
        void * RemoveObject(int id);

        CMapToPointer() {
        }

        virtual ~CMapToPointer() {
        }
    };

    template < typename T > T obj_cast(void *p) {
        return reinterpret_cast<T> (p);
    }

}
#endif	/* _CMAPTOPOINTER_H */

