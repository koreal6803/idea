// **************************************************************************
// File       [ pat_file.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/09/13 created ]
// **************************************************************************

#ifndef _INTF_PATTERN_FILE_H_
#define _INTF_PATTERN_FILE_H_

#include <list>
#include <fstream>
#include <iostream>
#include <string.h>

namespace IntfNs {

enum PatClkType { PAT_CLK_HOLD, PAT_CLK_CAPT };


struct PatCycle {
    char       *pi;
    char       *po;
    PatClkType type;
};

typedef std::list<char *>     PatNameList;
typedef std::list<PatCycle *> PatCycleList;

class PatFile {
public:
            PatFile();
    virtual ~PatFile();

    virtual bool read(const char * const fname , const bool &verbose = false);
    virtual bool parseName(const char * const fname);

    virtual bool setPiOrder(PatNameList * const pis);
    virtual bool setPoOrder(PatNameList * const pos);
    virtual bool setScanOrder(PatNameList * const scans);

    virtual bool addPattern(const unsigned &index
        , const unsigned &nframe
        , PatCycleList * const cycles
        , const char * const ppi
        , const char * const ppo);

protected:
    bool verbose_;
};

inline PatFile::PatFile() {
    verbose_ = false;
}

inline PatFile::~PatFile() {}

};


#endif


