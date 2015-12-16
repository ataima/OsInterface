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
#include <vector>
#include <map>
#include "../inc/IMapToPointer.h"
#include "../inc/CMapToPointer.h"
#include "../inc/hardware_configure.h"



namespace utility {

    long int CMapToPointer::UID_HANDLE = 0;

    void * CMapToPointer::PointerFromId(int id) {
        if (imap.size() == 0) {
            return NULL;
        } else {
            idToPtr::iterator it = imap.find(id);
            if (it == imap.end()) {
                return NULL;
            } else {
                return it->second;
            }
        }
    }

    int CMapToPointer::IdFromPointer(void *P) {
        if (pmap.size() == 0) {
            return 0;
        } else {
            ptrToId::iterator it = pmap.find(P);
            if (it == pmap.end()) {
                return 0;
            } else {
                return it->second;
            }
        }
    }

    int CMapToPointer::AddObject(void *ptr) {
        UID_HANDLE++;
        int index = UID_HANDLE + BASE_HANDLE;
        imap[index] = ptr;
        pmap[ptr] = index;
        return index;
    }

    void * CMapToPointer::RemoveObject(int id) {
        if (imap.size() == 0) {
            return NULL;
        } else {
            idToPtr::iterator it = imap.find(id);
            if (it == imap.end()) {
                return NULL;
            } else {
                void *ptr = it->second;
                imap.erase(it);
                ptrToId::iterator pit = pmap.find(ptr);
                if (pit == pmap.end()) {
                    return NULL;
                } else {
                    pmap.erase(pit);
                    return ptr;
                }
            }
        }
    }
}
