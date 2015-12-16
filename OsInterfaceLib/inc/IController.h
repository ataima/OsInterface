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

#ifndef ICONTROLLER_H
#define	ICONTROLLER_H


#include <list>
#include <string>

class IRule {
    /* a empty class to defined in derivate class with Specialization */
public:

    virtual ~IRule() {
    }
};

typedef std::list<IRule *> IRuleList;

class IController {
public:
    /** execute the control refered on rules */
    virtual int CheckControl(IRuleList * rules) = 0;
    /** initialize the object */
    virtual int Start(void) = 0;
    /** close the object */
    virtual int Close(void) = 0;
    /** container for specific implementation function*/
    virtual int getParam(int off, std::string & out) = 0;

    /** virtual dtor needed */
    virtual ~IController() {
    }

};

#endif	/* ICONTROLLER_H */

