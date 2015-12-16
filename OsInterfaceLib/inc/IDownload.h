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
#ifndef IDOWNLOADER_H
#define	IDOWNLOADER_H


typedef void *(*signalling_hook)(void *);

/*! Interface to downloader */
class IDownload {
    /** initialize the object and set the IO port*/
public:
    virtual int Open(IPort * RdWr, size_t bsize = 0x4000, bool debug = false) = 0;

    virtual int Recv(void) = 0;

    virtual int Xmit(const char *src_file, const char * dest_file) = 0;

    virtual int Xmit(unsigned char *membuff, size_t buff_size, const char * dest_file, signalling_hook hook = NULL) = 0;

    virtual int UserRecv(void *buff, size_t & size) = 0;

    virtual int UserXmit(void *buff, size_t & size) = 0;

    virtual int replyNack(size_t error) = 0;

    virtual ~IDownload() {
    }

};
#endif	/* IDOWNLOADER_H */

