
#include <stdlib.h>
#include <stdio.h> 
#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <fcntl.h>
#include <sstream>
#ifdef USE_LINUX
#include <sys/stat.h>
#endif
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/IBlock.h"
#include "../inc/IPort.h"
#include "../inc/IFile.h"
#include "../inc/CBasePort.h"
#include "../inc/CBaseBlock.h"
#include "../inc/IDownload.h"
#include "../inc/CBaseDownloader.h"
#include "../inc/CFileDownloader.h"
#include "../inc/CMapToPointer.h"
#include "../inc/hardware_configure.h"
#include "../inc/CLinuxPort.h"
#include "../inc/CWindowsPort.h"
#include "../inc/CLinuxFile.h"
#include "../inc/CWindowsFile.h"
#include "CGenericTestPort.h"

#define PATH "/home/ubuntu/test"
//define DUMP 1
int sem_end;


int sem_sincro_tx;
int sem_sincro_rx;



class matrixPort
: public test::CGenericTestPort {
    unsigned char *txbuff;
    unsigned char *rxbuff;
    size_t sizeBuff;
    int semphwr;
    int semphrd;
    size_t *ind_tx, *ind_rx;
public:

    matrixPort(unsigned char *_txbuff, unsigned char *_rxbuff, 
            int size, int semwr, int semrd, size_t * pos_tx,size_t * pos_rx) :
     sizeBuff(size), semphwr(semwr), semphrd(semrd) {
        txbuff=_txbuff;
        rxbuff=_rxbuff;
        ind_tx=pos_tx;
        ind_rx=pos_rx;
    }

    ~matrixPort()
    {
    }
    
    int WriteAndRead(unsigned char *out, unsigned char *in, unsigned int size, unsigned int & readed, unsigned int timeout) {
        return 0;
    }

    int Write(unsigned char *out, size_t size, size_t & writed, size_t timeout) {
        LOG(logger,"WRITE IND PRE =%d\n",*ind_tx);
        LOG(logger,"WRITE REQ SIZE =%d\n",size);
        if (size > sizeBuff)
            size = sizeBuff;
        memcpy(&txbuff[*ind_tx],out,size);
        *ind_tx+=size;
        writed = size;
        if (timeout != 0) {
            software->SleepThread(timeout);
        }
#ifdef DUMP
        if(logger!=NULL)
        {
            logger->dump("WRITE :",txbuff,size);
            logger->printf("WRITE BUFF IS : %08X -%08X - IND = %d\n",this,rxbuff,*ind_tx);
            
        }
#endif        
        software->SemphPost(semphwr);
        
        return 0;
    }

    int Read(unsigned char *in, size_t size, size_t & readed, size_t timeout) {
        software->SemphWait(semphrd);
#ifdef DUMP
        if(logger!=NULL)
        {
            logger->printf("READ BUFF IS : %08X -%08X - IND = %d\n",this,rxbuff,*ind_rx);
            logger->dump("READ :",rxbuff,size);
            
        }
#endif        
        LOG(logger,"READ REQ SIZE =%d\n",size);
        if (size > sizeBuff)
            size = sizeBuff;
        memcpy(in,rxbuff,size);
        memcpy(rxbuff,&rxbuff[size],sizeBuff-size);
        *ind_rx-=size;
        readed = size;
        if (timeout != 0) {
            software->SleepThread(timeout);
        }
        LOG(logger,"READ IND POST =%d\n",*ind_rx);
        return 0;
    }

};

class CTestMatrix
: public device::CBaseBlock {
    unsigned char *txbuff;
    unsigned char *rxbuff;
    matrixPort *pMaster;
    matrixPort *pSlave;
    size_t ind_tx,ind_rx;
public:

    CTestMatrix(size_t block_size) {
        ind_tx=ind_rx=0;
        txbuff = new unsigned char [2*block_size];
        rxbuff = new unsigned char [2*block_size];
        pMaster = new matrixPort(txbuff, rxbuff, 2*block_size, sem_sincro_tx, sem_sincro_rx,&ind_tx,&ind_rx);
        pSlave = new matrixPort(rxbuff, txbuff, 2*block_size, sem_sincro_rx, sem_sincro_tx,&ind_rx,&ind_tx);
        pMaster->Open();
        pSlave->Open();
    }

    ~CTestMatrix() {
        delete []txbuff;
        delete []rxbuff;
        delete pMaster;
        delete pSlave;
    }

    IPort * getMasterPort(void) {
        return pMaster;
    }

    IPort * getSlavePort(void) {
        return pSlave;
    }

};

class CTestDownloaderMaster
: public utility::downloader::CFileDownloader {
public:

    CTestDownloaderMaster(size_t a, size_t b) : utility::downloader::CFileDownloader(a, b) {
    }
    void handler(void);
};

class CTestDownloaderSlave
: public utility::downloader::CFileDownloader {
public:

    CTestDownloaderSlave(size_t a, size_t b) : utility::downloader::CFileDownloader(a, b) {
    }
    void handler(void);
};


//thread trasmitter

void * handlerXmit(void *p) {
    CTestDownloaderMaster *xmitter = ::utility::obj_cast<CTestDownloaderMaster *>(p);
    if (xmitter != NULL) {
        xmitter->handler();
    }
}


//thread receiver

void * handlerRecv(void *p) {
    CTestDownloaderSlave *recver = ::utility::obj_cast<CTestDownloaderSlave *>(p);
    if (recver != NULL) {
        recver->handler();
    }
}

void CTestDownloaderMaster::handler(void) {
    Xmit(PATH"/test.xmit", PATH"/test.recv");
    software->SemphPost(sem_end);
    software->SleepThread(2000);
    LOG(logger, "Master thread ended\n")
}

void CTestDownloaderSlave::handler(void) {
    Recv();
    software->SemphPost(sem_end);
    software->SleepThread(2000);
    LOG(logger, "Slave thread ended\n")
}

int main(int argc, char** argv) {
    int mth_tx, mth_rx;
    device::CGenericDevice::Create("stderr");
    IDevice * device = device::CDefaultDevice::getDevice();
    ILogger *logger = device->GetAvaiableLogger();
    ISoftware *software = device->GetAvaiableSoftware();
    software->CreateSemph(sem_end, 0);
    software->CreateSemph(sem_sincro_tx, 0);
    software->CreateSemph(sem_sincro_rx, 0);
    CTestMatrix matrix(0x3fff);
    IPort *testPort1 = matrix.getMasterPort();
    IPort *testPort2 = matrix.getSlavePort();
    hardware::CFile file;
    mkdir(PATH, S_IXUSR | S_IXGRP | S_IROTH);
    if (file.Open(PATH"/test.xmit") == 0) {
        unsigned char buff[256];
        int i;
        size_t writed;
        for (i = 0; i < 256; i++)
            buff[i] = i;
        for (i = 0; i < 257; i++)
            file.Write(buff, 256, writed, 0);
        file.Close();
    } else {
        return -1;
    }
    CTestDownloaderMaster xmitter(0xcacababa, 0xbabacaca);
    CTestDownloaderSlave recver(0xcacababa, 0xbabacaca);
    if (xmitter.Open(testPort1, 0x3fff, true) == 0) {
        if (recver.Open(testPort2, 0x3fff, true) == 0) {
            software->CreateThread((void *) handlerXmit, &xmitter, mth_tx);
            software->CreateThread((void *) handlerRecv, &recver, mth_rx);
            software->SemphWait(sem_end);
            software->DestroyThread(mth_tx);
            software->DestroyThread(mth_rx);
        } else {
            LOG(logger, "Cannot open recver object\n");
        }
    } else {
        LOG(logger, "Cannot open Xmitter object\n");
    }
    software->DestroySemph(sem_sincro_rx);
    software->DestroySemph(sem_sincro_tx);
    software->DestroySemph(sem_end);
    return (EXIT_SUCCESS);
}

