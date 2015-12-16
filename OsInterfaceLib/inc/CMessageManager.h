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

#ifndef _CMESSAGEMANAGER_H
#define	_CMESSAGEMANAGER_H



#include <vector>
#include <map>


namespace messages {

    class CMessageManager
    : public CGenericMessageManager {
        static int UID;
        IMessageFactory *factory;
    public:
        CMessageManager();
        ~CMessageManager();
        // create a right msg from message_type, timeout = live msg in ms,
        //_data ->field data on msg
        IMessage *CreateMsg(const unsigned int message_type, unsigned int timeout = 0, int numRetry = 0);
        void DestroyMsg(IMessage ** msg);
        int setFactory(IMessageFactory *nfactory);
        IMessageFactory * getFactory(void);


    };
}



#endif	/* _CMESSAGEMANAGER_H */

