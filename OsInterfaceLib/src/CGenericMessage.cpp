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
#include "../inc/ITime.h"
#include "../inc/CGenericTime.h"
#include "../inc/IMessage.h"
#include "../inc/IMessageFactory.h"
#include "../inc/IMessageManager.h"
#include "../inc/CGenericMessage.h"
#include "../inc/CGenericMessageManager.h"


namespace messages {

    CGenericMessage::CGenericMessage() : data(NULL), nID(-1),
    timeout(1000), numRetry(3), curRetry(0), message_type(-1) {
        memset(&startTime, 0, sizeof (msgTime));
        memset(&stopTime, 0, sizeof (msgTime));
    }

    CGenericMessage::~CGenericMessage() {
        nID = timeout = numRetry = curRetry = message_type = 0;
        data = NULL;
        memset(&startTime, 0, sizeof (msgTime));
        memset(&stopTime, 0, sizeof (msgTime));
    }

    size_t CGenericMessage::toString(std::string &out, int details) {
        std::stringstream str;
        str << "message_type=" << message_type << " : nID=" << nID <<
                "-" << "Time=" << startTime.sec <<
                ":" << startTime.milli <<
                ":" << stopTime.sec << ":" << stopTime.milli <<
                "-" << numRetry << ":" << curRetry << std::endl;
        if (details != 0 && data != NULL) {
            std::string f;
            if (data->toString(f) > 0) {
                str << f;
            }
        }
        out = str.str();
        return out.size();
    }

    void CGenericMessage::createUTime(CGenericMessage & msg, size_t timeout) {
        CGenericTime::getZeroTime()->getTime();
        msg.startTime.sec = CGenericTime::getZeroTime()->getSecond();
        msg.startTime.milli = CGenericTime::getZeroTime()->getMilliSec();
        msg.stopTime = msg.startTime;
        msg.timeout = timeout;
    }

    IMessage::msgStatusResponse CGenericMessage::refresh(CGenericMessage & msg, size_t _time) {
        msgStatusResponse res = response_ok;

        msgTime r;
        r.sec = _time / 1000;
        r.milli = (_time % 1000);
        //stop+r
        msg.stopTime.milli += r.milli;
        msg.stopTime.sec += r.sec;
        if (msg.stopTime.milli >= 1000) {
            msg.stopTime.milli -= 1000;
            msg.stopTime.sec += 1;
        }
        //stop>start
        if ((msg.stopTime.sec > msg.startTime.sec) ||
                ((msg.stopTime.sec == msg.startTime.sec) && (msg.stopTime.milli > msg.startTime.milli))) {
            r.sec = msg.stopTime.sec - msg.startTime.sec;
            r.milli = msg.stopTime.milli - msg.startTime.milli;
            if (r.milli > 1000) {
                r.milli += 1000;
                r.sec--;
            }
            if (msg.curRetry < msg.numRetry) {
                CGenericMessage::createUTime(msg, msg.timeout);
                msg.startTime.milli += r.milli;
                msg.startTime.sec += r.sec;
                if (msg.startTime.milli >= 1000) {
                    msg.startTime.milli -= 1000;
                    msg.startTime.sec += 1;
                }
                msg.curRetry++;
                res = response_to_send;
            } else {
                res = response_failed;
            }

        }

        return res;
    }
}


