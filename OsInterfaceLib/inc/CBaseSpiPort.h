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

#ifndef BASESPIPORT_H
#define	BASESPIPORT_H

class CBaseSpiPort
: public ISpiPort {
protected:
    char filename[255];
    spiSetting setting;
    int numPort;
public:

    CBaseSpiPort(int nPort) {
        memset(filename, 0, sizeof (filename));
        memset(&setting, 0, sizeof (spiSetting));
        numPort = nPort;
    }

    virtual ~CBaseSpiPort() {
        memset(filename, 0, sizeof (filename));
        memset(&setting, 0, sizeof (spiSetting));
        numPort = -1;
    }

    inline int GetPortNumber(void) {
        return numPort;
    }

    inline spiSetting * GetSetting(void) {
        return &setting;
    }

    inline char * GetPortname(void) {
        return filename;
    }

};


#endif	/* CBASESPIPORT_H */

