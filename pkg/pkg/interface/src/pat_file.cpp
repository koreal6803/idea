// **************************************************************************
// File       [ pat_file.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/09/13 created ]
// **************************************************************************

#include <cstdio>
#include "pat_file.h"

using namespace std;
using namespace IntfNs;

extern int pat_fileparse(void *);
extern FILE *pat_filein;
    
bool PatFile::read(const char * const fname, const bool &verbose) {
    verbose_ = verbose;
    pat_filein = fopen(fname, "r");
    if (!pat_filein) {
        fprintf(stderr, "**ERROR PatternFile::read(): cannot open pattern ");
        fprintf(stderr, "file `%s'\n", fname);
        return false;
    }

    int res = pat_fileparse(this);
    if (res != 0) {
        fprintf(stderr, "**ERROR PatternFile::read(): wrong input format\n");
        fclose(pat_filein);
        return false;
    }
    fclose(pat_filein);
    // parse PI PO SCAN ORDER
    parseName(fname);

    return true;;
}
bool PatFile::parseName(const char* const fname){
    return true;
}

bool PatFile::setPiOrder(PatNameList * const pis) {
    if (!verbose_)
        return true;
    printf("set PI order: ");
    PatNameList::iterator it = pis->begin();
    for ( ; it != pis->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool PatFile::setPoOrder(PatNameList * const pos) {
    if (!verbose_)
        return true;
    printf("set PO order: ");
    PatNameList::iterator it = pos->begin();
    for ( ; it != pos->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool PatFile::setScanOrder(PatNameList * const scans) {
    if (!verbose_)
        return true;
    printf("set scan order: ");
    PatNameList::iterator it = scans->begin();
    for ( ; it != scans->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool PatFile::addPattern(const unsigned &index
    , const unsigned &nframe
    , PatCycleList * const cycles
    , const char * const ppi
    , const char * const ppo)
{
    if (!verbose_)
        return true;
    printf("add pattern: %u %u\n", index, nframe);
    PatCycleList::iterator it = cycles->begin();
    for ( ; it != cycles->end(); ++it) {
        printf("    PIs %s  POs %s ", (*it)->pi, (*it)->po);
        if ((*it)->type == PAT_CLK_CAPT)
            printf("[capture]\n");
        else
            printf("[hold]\n");
    }
    if (ppi && ppo)
        printf("    PPI %s  PPO %s", ppi, ppo);
    printf("\n");
    return true;
}

