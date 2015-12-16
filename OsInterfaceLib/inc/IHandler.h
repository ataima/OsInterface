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
#ifndef IHANDLER_H
#define	IHANDLER_H

/* simple class to manage a function hook  on a thread */

/* you have to derive a class for this and implement a specific
 * hook function with the right parameters*/
class IHandler {
public:

    virtual int hook(void) = 0;

    virtual void setSoftware(ISoftware *_s) = 0;

    virtual void setLogger(ILogger *_l) = 0;

    virtual ~IHandler() {
    }
};

#endif	/* IHANDLER_H */

