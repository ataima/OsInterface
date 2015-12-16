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
#ifdef USE_WINDOWS
#include <windows.h>
#include <stdio.h>
#include <string.h>

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
#include "../inc/ICommPort.h"
#include "../inc/CBaseCommPort.h"
#include "../inc/CWindowsCommPort.h"



namespace hardware {

    CCommPort::CCommPort(int _numport, const commSetting & _setting):CBaseCommPort (_numport,_setting){
    }

    CCommPort::~CCommPort() {
		if (mHandle != -1)
		{
			CloseHandle((HANDLE)mHandle);
			mHandle = -1;
		}
    }


	int CCommPort::SetFilenameFromPort(void)
	{
		if (this->numPort < 10)
			sprintf(filename, "COM%d", numPort);
		else
			sprintf(filename, "\\\\.\\COM%d", numPort);
		return 0;
	}

    int CCommPort::Open(const commSetting * _setting) {
        int res = -1;
		if (mHandle != -1)
			res = comm_already_open_error;
		else
		{
		HANDLE handleDev = INVALID_HANDLE_VALUE;
		SetFilenameFromPort();
		handleDev = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, INVALID_HANDLE_VALUE);
		if (handleDev != INVALID_HANDLE_VALUE) {
			LOG(logger, "%s:%d:fd : %d(%s)\n", __FUNCTION__, __LINE__, mHandle, filename);
			if (_setting != NULL){
				memcpy(&setting, &_setting, sizeof(commSetting));
			}
			// Legge i dati di default ed inizialicca il COMMPROP locale
			if (GetCommProperties(handleDev, &deviceProperties) == FALSE)
			{
				LOG(logger,"%s:%d :GetCommProperties Fail\n",__FUNCTION__,__LINE__);
				res = comm_attribute_error;
				CloseHandle(handleDev);
				goto init_exit;
			}

			// Legge i dati di default ed inizialicca il DCB locale
			setupDCB.DCBlength = sizeof(DCB);
			if (GetCommState(handleDev, &setupDCB) == FALSE) {
				LOG(logger, "%s:%d :GetCommState Fail\n", __FUNCTION__, __LINE__);
				LOG(logger, "--> Warning: manual reinit of DCB.r\n");
				setupDCB.fBinary = TRUE;
				setupDCB.fParity = TRUE;
				setupDCB.fOutxCtsFlow = FALSE;
				setupDCB.fOutxDsrFlow = FALSE;
				setupDCB.fDtrControl = DTR_CONTROL_DISABLE;
				setupDCB.fDsrSensitivity = FALSE;
				setupDCB.fTXContinueOnXoff = FALSE;
				setupDCB.fOutX = FALSE;
				setupDCB.fInX = FALSE;
				setupDCB.fErrorChar = FALSE;
				setupDCB.fNull = FALSE;
				setupDCB.fRtsControl = RTS_CONTROL_DISABLE;
				setupDCB.fAbortOnError = FALSE;
				setupDCB.wReserved = 0;
				setupDCB.XonLim = 0;
				setupDCB.XoffLim = 0;
				setupDCB.XonChar = 0;
				setupDCB.XoffChar = 0;
				setupDCB.ErrorChar = 0x00;
				setupDCB.EofChar = 0;
				setupDCB.EvtChar = 0;
			}
			setupDCB.BaudRate = setting.baudrate;
			setupDCB.ByteSize = setting.databit;
			setupDCB.Parity = setting.parity;
			setupDCB.StopBits = setting.stop;
			setupDCB.fOutxCtsFlow = FALSE;
			setupDCB.fOutxDsrFlow = FALSE;
			if (setting.ctsrts == false)
				setupDCB.fDtrControl = DTR_CONTROL_DISABLE;
			else
				setupDCB.fDtrControl = DTR_CONTROL_ENABLE;
			setupDCB.fDsrSensitivity = FALSE;
			if (setting.ctsrts == false)
				setupDCB.fRtsControl = RTS_CONTROL_DISABLE;
			else
				setupDCB.fRtsControl = RTS_CONTROL_ENABLE;
			// Specify time-out between charactor for receiving.
			comTimeOut.ReadIntervalTimeout = 3;
			// Specify value that is multiplied 
			// by the requested number of bytes to be read. 
			comTimeOut.ReadTotalTimeoutMultiplier = 3;
			// Specify value is added to the product of the 
			// ReadTotalTimeoutMultiplier member
			comTimeOut.ReadTotalTimeoutConstant = 2;
			// Specify value that is multiplied 
			// by the requested number of bytes to be sent. 
			comTimeOut.WriteTotalTimeoutMultiplier = 3;
			// Specify value is added to the product of the 
			// WriteTotalTimeoutMultiplier member
			comTimeOut.WriteTotalTimeoutConstant = 2;
			// set the time-out parameter into device control.
			if (SetCommTimeouts(handleDev, &comTimeOut) == FALSE) {
				LOG(logger, "%s:%d: SetCommTimeouts Fail\n", __FUNCTION__, __LINE__);
				CloseHandle(handleDev);
				res = comm_setconfig_error;
				goto init_exit;
			}

			if (SetCommState(handleDev, &setupDCB) == FALSE) {
				LOG(logger, "%s:%d: SetCommState Fail\n",__FUNCTION__,__LINE__);
				CloseHandle(handleDev);
				res = comm_setconfig_error;
				goto init_exit;
			}


		}
		else {
			LOG(logger, "%s:%d: Open device %s Fail\n", filename);
			res = comm_open_error;
		}
		mHandle = (int)handleDev;

		}
	init_exit:
		return res;
    }

    int CCommPort::Close(void) {
        int res = -1;
		if (CloseHandle((HANDLE)mHandle) != 0)
			res = 0;
        return res;
    }

    int CCommPort::Flush(void) {
        int res = -1;
		if (mHandle != -1){
			if (PurgeComm((HANDLE)mHandle, PURGE_RXCLEAR | PURGE_TXCLEAR) != 0)
				res = 0;
		}
        return res;
    }

}

#endif