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

#ifndef CTESTSPIPORT_H
#define	CTESTSPIPORT_H

namespace hardware {
    namespace test {

        class CSpiPort
        : public ISpiPort {
            unsigned char lastNotifyResp[1280];
            volatile unsigned short lastNotifySize;
            int mph_resp;
            int mth_resp;
            int mtx_com;
            int mth_notify;
            int mode;
        public:
            CSpiPort(int nPort, const spiSetting & setting);
            ~CSpiPort();
            int Open(spiSetting * setting = NULL);
            int Close(void);
            int Flush(void);
            int Write(unsigned char *out, unsigned int size, unsigned int & writed);
            int Read(unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout);
            int WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout);
            int XDatamit(CSpiPort *p);
            int TxNotify(CSpiPort *p);

            inline int GetCommMutex(void) {
                return mtx_com;
            }
        };
    }
}


#endif	/* CTESTSPIPORT_H */

