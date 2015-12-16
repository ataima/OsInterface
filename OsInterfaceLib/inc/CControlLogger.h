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

#ifndef CCONTROLLOGGER_H
#define	CCONTROLLOGGER_H

#include "CBaseController.h"
#include <vector>

/* a class to control the size occupied on a ptah an num files created*/

namespace utility {

    class CCircularLogFileNames {
        size_t WIDTH;
        size_t start;
        size_t stop;
    public:

        CCircularLogFileNames(size_t size = 1000) {
            WIDTH = size;
            start = 0;
            stop = 1;
        }

        inline void SetWidth(size_t size) {
            WIDTH = size;
            start = 0;
            stop = 1;
        }

        inline bool Empthy(void) {
            return start == stop;
        }

        inline bool Insert(int i) {
            if (start == 0)
                start = (i % WIDTH);
            stop = (i % WIDTH);
            //printf("CCircularLogFileNames::Insert :%d %d:%d\n", i, start, stop);
            return true;
        }

        inline bool Remove(void) {
            start++;
            start = (start % WIDTH);
            if (start == 0)
                start++;
            //printf("CCircularLogFileNames::Remove : %d:%d\n", start, stop);
            return true;
        }


        int LoadFromHistory(std::string path, FILE * file);

        int getNextIndexFile(void) {
            int i;
            i = stop + 1;
            i %= WIDTH;
            if (i == 0)
                i++;
            return i;
        }

        inline void getPosition(size_t &r_start, size_t & r_stop) {
            r_start = start;
            r_stop = stop;
        }

        inline int getOldestIndexFile(void) {
            return start;
        }

        inline int getCurrentIndexFile(void) {
            return stop;
        }

        inline size_t Size(void) {
            if (start == 0 && stop == 1)
                return 0;
            else
                if (stop == start)
                return 1;
            else {
                if (stop > start)
                    return (stop - start) + 1;
                else
                    return (WIDTH - start + stop) + 1;
            }
        }

        inline void Clean(void) {
            start = stop = 0;
        }

    };

    class CDirRules
    : public IRule {
    public:
        std::string history;
        std::string pathname;
        size_t max_path_size;
        size_t max_num_file;

    public:
        CDirRules(const char * path = NULL, const char * history = NULL, size_t max_size = 0, size_t max_file = 0);
        virtual ~CDirRules();
    };

    class CControlLogger
    : public CBaseController {
        int mth_service;
        int mtx_files;
        utility::CDirRules cur_rule;
        CCircularLogFileNames cur_index;
    public:

        typedef enum tag_control_logger_error {
            control_logger_generic_error = -1,
            control_logger_not_initialized = -70000,
            control_logger_no_valid_rules, // -69999
            control_logger_no_start_service, // -69998
            control_logger_create_mutex // -69997
        } control_logger_error;

    public:
        virtual void * handlerRunControl(CControlLogger * p);
    public:

        CControlLogger();

        virtual ~CControlLogger();

        int CheckControl(IRuleList * rules);

        int Start(void);

        int Close(void);

        int getNextFileName(std::string & out);

        int getOldestFileName(std::string & out);

        int getCurrentFileName(std::string & out);

        int getParam(int off, std::string & out);

    };


}
#endif	/* CCONTROLLOGGER_H */

