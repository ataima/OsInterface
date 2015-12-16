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

#ifndef _BASECOMMPORT_H
#define	_BASECOMMPORT_H

class CBaseCommPort
: public ICommPort {
protected:
    char filename[255];
    commSetting setting;
    int numPort;
public:

    CBaseCommPort(int nPort, const commSetting & _setting) {
        memcpy(&setting, &_setting, sizeof (commSetting));
		memset(filename, 0, sizeof(filename));
        numPort = nPort;
    }

    virtual ~CBaseCommPort() {
        memset(filename, 0, sizeof (filename));
        memset(&setting, 0, sizeof (commSetting));
        numPort = -1;
    }

    inline int GetPortNumber(void) {
        return numPort;
    }

    inline commSetting * GetSetting(void) {
        return &setting;
    }

    inline char * GetPortName(void) {
        return filename;
    }

	

};

#endif	/* _IPORT_H */

