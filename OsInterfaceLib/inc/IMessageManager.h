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
#ifndef _IMESSAGEMANAGER_H
#define	_IMESSAGEMANAGER_H

class IMessageFactory;

class IMessageManager {
public:

    virtual IMessage *CreateMsg(const unsigned int id, unsigned int timeout = 0, int numRetry = 0) = 0;
    virtual void DestroyMsg(IMessage ** msg) = 0;
    virtual int setFactory(IMessageFactory *factory) = 0;
    virtual IMessageFactory * getFactory(void) = 0;

    virtual ~IMessageManager() {
    }
};


#endif	/* _IMESSAGEMANAGER_H */

