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

#ifdef USE_LINUX
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>


#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#ifdef __CYGWIN__
// only for compile on CYGWIN not work really on cygwin
#define  SPI_IOC_WR_MODE            10   
#define  SPI_IOC_RD_MODE            11
#define  SPI_IOC_WR_BITS_PER_WORD   12 
#define  SPI_IOC_RD_BITS_PER_WORD   13
#define  SPI_IOC_WR_MAX_SPEED_HZ    14
#define  SPI_IOC_RD_MAX_SPEED_HZ    15
#else // linux
#ifdef __MACH__
// only for compile on CYGWIN not work really on cygwin
#define  SPI_IOC_WR_MODE            10   
#define  SPI_IOC_RD_MODE            11
#define  SPI_IOC_WR_BITS_PER_WORD   12 
#define  SPI_IOC_RD_BITS_PER_WORD   13
#define  SPI_IOC_WR_MAX_SPEED_HZ    14
#define  SPI_IOC_RD_MAX_SPEED_HZ    15
#else
#include <linux/types.h>
#include <linux/spi/spidev.h>
#endif
#endif 

#include "../inc/IPort.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/ILogger.h"
#include "../inc/ISpiPort.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/IDevice.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CDefaultHardware.h"
#include "../inc/CBaseSpiPort.h"
#include "../inc/CSpiPort.h"


namespace hardware {

    /**
     * default costructor
     * @param nPort : spi port number
     * @param setting : spi parameter 
     */
    CSpiPort::CSpiPort()
    : CBaseSpiPort(-1) {
        mHandle = -1;
    }

    /**
     * defaut desctructor
     */
    CSpiPort::~CSpiPort() {
        mHandle = -1;
    }

    /**
     * Open spi port
     * @param setting : parameter setting
     * @return 0 OK 
     */
    int CSpiPort::Open(int nPort, const char *_filename, const spiSetting & newsetting) {
        ::device::CBaseBlock::Initialize();
        unsigned char mode = 0;
        int res = spi_generic_error;
        numPort = nPort;
        strncpy(filename, _filename, sizeof (filename) - 1);
        memcpy(&setting, &newsetting, sizeof (spiSetting));
        if (mHandle != -1)
            res = spi_already_open_error;
        else {
            mHandle = open(filename, O_RDWR);
            if (mHandle == -1) {
                LOG(logger, "%s:%d:Spi Port %d Cannot open : %s", __FUNCTION__, __LINE__, numPort, filename);
                res = spi_open_error;
            } else {
                res = ioctl(mHandle, SPI_IOC_WR_MODE, &mode);
                if (res == 0) {
                    res = ioctl(mHandle, SPI_IOC_RD_MODE, &mode);
                    if (res == 0) {
                        res = ioctl(mHandle, SPI_IOC_WR_BITS_PER_WORD, &setting.datalen);
                        if (res == 0) {
                            res = ioctl(mHandle, SPI_IOC_RD_BITS_PER_WORD, &setting.datalen);
                            if (res == 0) {
                                res = ioctl(mHandle, SPI_IOC_WR_MAX_SPEED_HZ, &setting.speed);
                                if (res == 0) {
                                    res = ioctl(mHandle, SPI_IOC_RD_MAX_SPEED_HZ, &setting.speed);
                                    if (res != 0) {
                                        res = spi_set_speed;
                                        LOG(logger, "%s:%d:Spi Port %d Cannot set rd speed len", __FUNCTION__, __LINE__, numPort);
                                    } else {
                                        LOG(logger, "%s:%d:Spi Port %d %s OPEN OK!", __FUNCTION__, __LINE__, numPort, filename);
                                    }
                                } else {
                                    res = spi_set_speed;
                                    LOG(logger, "%s:%d:Spi Port %d Cannot set wr speed len", __FUNCTION__, __LINE__, numPort);
                                }

                            } else {
                                res = spi_set_bit_len;
                                LOG(logger, "%s:%d:Spi Port %d Cannot set rd bit len", __FUNCTION__, __LINE__, numPort);
                            }
                        } else {
                            res = spi_set_bit_len;
                            LOG(logger, "%s:%d:Spi Port %d Cannot set wr bit len", __FUNCTION__, __LINE__, numPort);
                        }

                    } else {
                        res = spi_set_rd_mode;
                        LOG(logger, "%s:%d:Spi Port %d Cannot set rd mode", __FUNCTION__, __LINE__, numPort);
                    }

                } else {
                    res = spi_set_wr_mode;
                    LOG(logger, "%s:%d:Spi Port %d Cannot set wr mode", __FUNCTION__, __LINE__, numPort);
                }
            }
        }
        return res;
    }

    int CSpiPort::Close(void) {
        int res = spi_generic_error;
        if (mHandle != -1) {
            if (close(mHandle) == 0) {
                res = 0;
            } else {
                LOG(logger, "%s:%d:Spi Port %d %s Close fail ! ", __FUNCTION__, __LINE__, numPort, filename);
                res = spi_close_error;
            }
            mHandle = -1;
        }
        return res;
    }

    int CSpiPort::Flush(void) {
        int res = spi_generic_error;
        if (write(mHandle, NULL, 0) == 0) {

            res = 0;
        } else {
            LOG(logger, "%s:%d:Spi Port %d %s flush fail ! ", __FUNCTION__, __LINE__, numPort, filename);
            res = spi_flush_error;
        }
        return res;

    }


}
#endif