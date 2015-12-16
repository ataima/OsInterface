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

#ifndef _CPROTOCOL_H
#define	_CPROTOCOL_H



namespace protocol {

    typedef utility::CCircularBuffer<IMessage* > msgQueue;

    class CProtocol
    : public IProtocol
    , public ::device::CBaseBlock {
    protected:
        int enableThreadRx;
        int enableThreadTx;
        volatile int rd_Port;
        volatile int wr_Port;
        /*******TX OBJECTS********/
        msgQueue Tx; //TX queue
        int thread_tx; // handle of Tx Thread 
        volatile bool exit_from_tx; //flags exit thread
        int mtx_Tx; // handle of mutex for insert/remove msg on TX 
        int mph_Tx; // handle of semaphore to signalling TX sem_post on dispatchMessage
        /*******RX OBJECTS********/
        msgQueue Rx; //RX queue
        int thread_rx; // handle of Rx Thread 
        volatile bool exit_from_rx; //flags exit thread
        int mtx_Rx; // handle of mutex for insert/remove msg on RX 
        int mph_Rx; // handle of semaphore to signalling RX sem_post on dispatchMessage
    protected:
        int CreateTxObject(void);
        int CreateRxObject(void);
        int DestroyTxObject(void);
        int DestroyRxObject(void);
    public:
        ///segnala un evento di TX al rispettivo thread
        virtual void event_Tx(void);
        ///segnala un evento di RX al rispettivo thread
        virtual void event_Rx(void);
        virtual void * handlerTrasmit(CProtocol * p);
        virtual void * handlerReceive(CProtocol * p) = 0;
    public:
        CProtocol();
        virtual ~CProtocol();
        virtual int dispatchMessage(IMessage *msg);
        virtual int listenerMessage(IMessage *msg);
        virtual int Open(int enableRX, int enableTX);
        virtual int Close(void);
        virtual IMessage * GetLastRxMessage(void);
        virtual void isRxEmpthy(int *res);

        virtual void setRxService(int rd) {
            //printf("CProtocol::setRxService file descr. - P:%d N:%d\n", rd_Port, rd);
            rd_Port = rd;
        }

        virtual void setTxService(int wr) {
            //printf("CProtocol::setTxService file descr. - P:%d N:%d\n", wr_Port, wr);
            wr_Port = wr;
        }

        virtual int getRxService(void) {
            return rd_Port;
        }

        virtual int getTxService(void) {
            return wr_Port;
        }

        virtual void setRxSemaphore(int mph) {
            if (VALID_HANDLE(mph))
                mph_Rx = mph;
        }


    };





}
#endif	/* _CPROTOCOL_H */

