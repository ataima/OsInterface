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
#ifndef _IMESSAGE_HEADER
#define	_IMESSAGE_HEADER

class IMessage {
public:

    typedef enum tag_msg_status_response {
        response_ok,
        response_to_send,
        response_failed
    } msgStatusResponse;

    typedef struct tag_msg_time {
        size_t sec;
        size_t milli;

        bool operator<(const tag_msg_time & b) {
            if (sec < b.sec) {
                if (milli < b.milli)
                    return true;
            }
            return false;
        }
    } msgTime;
public:
    virtual IRawData * getData() = 0;
    virtual void setData(IRawData *) = 0;
    virtual size_t getSequenceID() = 0;
    virtual msgTime & getStartTime() = 0;
    virtual msgTime & getStopTime() = 0;
    virtual size_t toString(std::string & out, int details = 0) = 0;
    virtual size_t getMessageType(void)const = 0;
    virtual size_t getNumRetry(void) = 0;
    virtual void setNumRetry(size_t num) = 0;
    virtual size_t getCurrentRetry(void) = 0;
    virtual void setSeqID(size_t _nID) = 0;
    virtual int getResult(void) = 0;

    virtual ~IMessage() {
    }
};



#endif	/* _IMESSAGE_H */

