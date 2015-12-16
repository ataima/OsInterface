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


#ifndef CBASE_CYPHER_H
#define	CBASE_CYPHER_H



#include "ICypher.h"
#include "CBaseBlock.h"

class CBaseCypher
: public ICypher
, public device::CBaseBlock {
public:

    CBaseCypher() : CBaseBlock() {
    }

    virtual ~CBaseCypher() {
    }

};


#endif	

