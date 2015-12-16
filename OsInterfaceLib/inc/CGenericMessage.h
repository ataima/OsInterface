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

#ifndef _CGENERICMESSAGE_HEADER
#define	_CGENERICMESSAGE_HEADER

#include <string>
#include <sstream>

#include "IRawData.h"
#include "CRawData.h"
#ifdef USE_WINDOWS
#include <winsock.h>
#endif 
namespace messages {

    class CGenericMessage
    : public IMessage {
    protected:
        IRawData *data;
        size_t nID;
        msgTime startTime;
        msgTime stopTime;
        size_t timeout;
        size_t numRetry;
        size_t curRetry;
        size_t message_type;
    public:

        CGenericMessage();

        virtual ~CGenericMessage();

        inline void setData(IRawData *_data) {
            data = _data;
        }

        inline IRawData * getData() {
            return data;
        }

        inline size_t getSequenceID() {
            return nID;
        }

        inline msgTime & getStartTime() {
            return startTime;
        }

        inline msgTime & getStopTime() {
            return stopTime;
        }

        virtual size_t getMessageType(void)const {
            return message_type;
        }

        virtual size_t toString(std::string & out, int details = 0);

        inline size_t getNumRetry(void) {
            return numRetry;
        }

        inline void setNumRetry(size_t num) {
            numRetry = num;
        }

        inline size_t getCurrentRetry(void) {
            return curRetry;
        }

        inline void setSeqID(size_t _nID) {
            nID = _nID;
        }

        inline int getResult(void) {
            return (int) (data->getAt(0));
        }

        static msgStatusResponse refresh(CGenericMessage & msg, size_t _time /* ms */);

        static void createUTime(CGenericMessage & msg, size_t timeout);

    };


}



#endif	/* _CGENERICMESSAGE_H */

