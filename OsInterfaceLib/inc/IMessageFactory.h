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
#ifndef IMESSAGEFACTORY_H
#define	IMESSAGEFACTORY_H

/*! This class implement the cretion of specific message from raw data
 *  For new protocol you have to implement the method create and toRaw fromRaw
 *  and set the factory to current IMessageManager
 */

class IMessageFactory {
public:
    virtual IMessage *CreateMsg(const size_t message_type) = 0;
    virtual void DestroyMsg(IMessage **msg) = 0;
    virtual int toRaw(unsigned char **out, size_t &size, IMessage *msg) = 0;
    virtual int fromRaw(size_t message_type, unsigned char *in, size_t size, IMessage **msg) = 0;

    virtual ~IMessageFactory() {
    }
};

#endif	/* IMESSAGEFACTORY_H */

