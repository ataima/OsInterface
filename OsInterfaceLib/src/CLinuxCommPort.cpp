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
#include "../inc/IPort.h"
#include "../inc/ILogger.h"
#include "../inc/ISoftware.h"
#include "../inc/IHardware.h"
#include "../inc/ICommPort.h"
#include "../inc/IDevice.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultDevice.h"
#include "../inc/IBlock.h"
#include "../inc/CBaseBlock.h"
#include "../inc/CBasePort.h"
#include "../inc/CPort.h"
#include "../inc/CBaseCommPort.h"
#include "../inc/CLinuxCommPort.h"
#include "../inc/CDefaultLogger.h"
#include "../inc/CGenericHardware.h"
#include "../inc/CGenericDevice.h"
#include "../inc/CDefaultHardware.h"



namespace hardware {

    CCommPort::CCommPort(int port,  const commSetting & _setting) :
    CBaseCommPort(port,  _setting) {
        mHandle = -1;
    }

    CCommPort::~CCommPort() {
		if(mHandle!=-1)
		{
			close(mHandle);
			mHandle=-1;
		}
    }

    unsigned int CCommPort::traslateConfigSpeed(unsigned int speed) {
        switch (speed) {
            case 0: return B0;
                break;
            case 50: return B50;
                break;
            case 75: return B75;
                break;
            case 110: return B110;
                break;
            case 134: return B134;
                break;
            case 150: return B150;
                break;
            case 200: return B200;
                break;
            case 300: return B300;
                break;
            case 600: return B600;
                break;
            case 1200: return B1200;
                break;
            case 1800: return B1800;
                break;
            case 2400: return B2400;
                break;
            case 4800: return B4800;
                break;
            case 9600: return B9600;
                break;
            case 19200: return B19200;
                break;
            case 38400: return B38400;
                break;
            case 57600: return B57600;
                break;
            case 115200: return B115200;
                break;
            case 230400: return B230400;
                break;
            case 460800: return B460800;
                break;
            case 500000: return B500000;
                break;
            case 576000: return B576000;
                break;
            case 921600: return B921600;
                break;
            case 1000000: return B1000000;
                break;
            case 1152000: return B1500000;
                break;
            case 2000000: return B2000000;
                break;
            case 2500000: return B2500000;
                break;
            case 3000000: return B3000000;
                break;
            case 3500000: return B3500000;
                break;
            case 4000000: return B4000000;
                break;
        }
        return speed;
    }

    unsigned int CCommPort::traslateConfigBit(unsigned int bits) {
        switch (bits) {
            case 5: return CS5;
                break;
            case 6: return CS6;
                break;
            case 7: return CS7;
                break;
            case 8: return CS8;
                break;
        }
        return bits;
    }

	int CCommPort::SetFilenameFromPort(void)
	{
		if (this->numPort < 10)
			sprintf(filename, "/dev/ttyS%d", numPort);
		else
		if (this->numPort < 20)
			sprintf(filename, "/dev/ttyACM%d", numPort-10);
		else
		if (this->numPort < 30)
			sprintf(filename, "/dev/ttyGS%d", numPort-20);
		else
			sprintf(filename, "/dev/tty%d", numPort-30);
		return 0;
	}



    int CCommPort::Open( const commSetting * newsetting) {
        ::device::CBaseBlock::Initialize();
        int res = comm_generic_error;
        if (mHandle != -1)
            res = comm_already_open_error;
        else {
			SetFilenameFromPort();
            if ((mHandle = open(filename, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
                LOG(logger, "%s:%d: %s\n", __FUNCTION__, __LINE__, filename);
                res = comm_open_error;
            } else {
                if (newsetting != NULL) {
                    memcpy(&setting, newsetting, sizeof (commSetting));
                }
                tcflush(mHandle, TCIOFLUSH);
                int n = fcntl(mHandle, F_GETFL, 0);
                n &= ~(O_NDELAY);
                fcntl(mHandle, F_SETFL, n);
                tcflush(mHandle, TCIOFLUSH);
                LOG(logger, "%s:%d:fd : %d(%s)\n", __FUNCTION__, __LINE__, mHandle, filename);
                if (tcgetattr(mHandle, &old_tio) == 0) {
                    bzero(&tio, sizeof (termios));
                    if (tcgetattr(mHandle, &tio) == 0) {
                        tio.c_cflag &= ~(CSIZE | PARENB | PARODD | CRTSCTS | CSTOPB);
                        tio.c_cflag = traslateConfigBit(setting.databit) | CLOCAL | CREAD;
                        if (setting.parity != 0) {
                            if (setting.parity == 1)
                                tio.c_cflag |= PARODD;
                            if (setting.parity == 2)
                                tio.c_cflag |= PARENB;
                        }
                        if (setting.stop == 2) {
                            tio.c_cflag |= CSTOPB;
                        }
                        if (setting.ctsrts == true) {
                            tio.c_cflag |= CRTSCTS;
                        }
                        tio.c_iflag = IGNBRK;
                        tio.c_oflag = 0;
                        tio.c_lflag = 0;
                        tio.c_cc[VTIME] = 0; /* inter-character timer unused */
                        tio.c_cc[VMIN] = 1; /* blocking read until 1000 char received */
                        if (cfsetospeed(&tio, traslateConfigSpeed(setting.baudrate)) == 0) {
                            if (cfsetispeed(&tio, traslateConfigSpeed(setting.baudrate)) == 0) {
                                if (tcsetattr(mHandle, TCSANOW, &tio) == 0) {
                                    if (tcflush(mHandle, 0) == 0) {
                                        int mcs = 0;
                                        ioctl(mHandle, TIOCMGET, &mcs);
                                        mcs |= TIOCM_RTS | TIOCM_CTS;
                                        ioctl(mHandle, TIOCMSET, &mcs);
                                        res = 0;
                                        LOG(logger, "%s:%d:Ready! \n", __FUNCTION__, __LINE__);
                                    } else {
                                        LOG(logger, "%s:%d:tcflush fail ! \n", __FUNCTION__, __LINE__);
                                        close(mHandle);
                                        mHandle = -1;
                                        res = comm_flush_error;
                                    }
                                } else {
                                    LOG(logger, "%s:%d:tcsetattr fail ! \n", __FUNCTION__, __LINE__);
                                    close(mHandle);
                                    mHandle = -1;
                                    res = comm_attribute_error;
                                }
                            } else {
                                LOG(logger, "%s:%d:tsetospeed fail ! \n", __FUNCTION__, __LINE__);
                                close(mHandle);
                                mHandle = -1;
                                res = comm_setconfig_error;
                            }

                        } else {
                            LOG(logger, "%s:%d:tsetospeed fail ! \n", __FUNCTION__, __LINE__);
                            close(mHandle);
                            mHandle = -1;
                            res = comm_setconfig_error;
                        }
                    } else {
                        LOG(logger, "%s:%d:tcgetattr fail ! \n", __FUNCTION__, __LINE__);
                        close(mHandle);
                        mHandle = -1;
                        res = comm_attribute_error;
                    }
                } else {
                    LOG(logger, "%s:%d:tcgetattr fail ! \n", __FUNCTION__, __LINE__);
                    close(mHandle);
                    mHandle = -1;
                    res = comm_attribute_error;
                }
            }
        }
        return res;
    }

    int CCommPort::Close(void) {
        int res = comm_generic_error;
        if (mHandle != -1) {
            if (close(mHandle) == 0) {
                res = 0;
            } else {
                LOG(logger, "%s:%d:Close fail ! \n", __FUNCTION__, __LINE__);
                res = comm_close_error;
            }
            mHandle = -1;
        }
        return res;
    }

    int CCommPort::Flush(void) {
        int res = comm_generic_error;
        if (tcflush(mHandle, 0) == 0) {
            res = 0;
        } else {
            LOG(logger, "%s:%d:tcflush fail ! \n", __FUNCTION__, __LINE__);
            res = comm_flush_error;
        }
        return res;
    }

}

#endif