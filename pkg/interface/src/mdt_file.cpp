// **************************************************************************
// File       [ mdt_file.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/06/23 created ]
// **************************************************************************

#include <cstdio>

#include "mdt_file.h"

using namespace std;
using namespace IntfNs;

extern int mdt_fileparse(void *);
extern int mdt_filewarning;
extern FILE *mdt_filein;

bool MdtFile::read(const char * const fname, const bool &verbose) {
    verbose_ = verbose;

    if (verbose_)
        mdt_filewarning = 1;
    else
        mdt_filewarning = 0;

    mdt_filein = fopen(fname, "r");
    if (!mdt_filein) {
        fprintf(stderr, "**ERROR MdtFile::read(): cannot open lib file");
        fprintf(stderr, "`%s'\n", fname);
        return false;
    }

    int res = mdt_fileparse(this);
    if (res != 0) {
        fprintf(stderr, "**ERROR MdtFile::read(): wrong input format\n");
        fclose(mdt_filein);
        return false;
    }
    fclose(mdt_filein);

    return true;
}


bool MdtFile::addModel(const char * const name) {
    if (!verbose_)
        return true;
    printf("add model: %s\n", name);
    return true;
}

bool MdtFile::addPorts(MdtNameList * const ports) {
    if (!verbose_)
        return true;
    printf("add ports: ");
    MdtNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool MdtFile::setInputNets(MdtNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set input nets: ");
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool MdtFile::setOutputNets(MdtNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set output nets: ");
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool MdtFile::setInoutNets(MdtNameList * const nets) {
    if (verbose_)
        return true;
    printf("set inout nets: ");
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool MdtFile::setInternNets(MdtNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set intern nets: ");
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool MdtFile::addPrimitive(const char * const type
    , const char * const name
    , MdtNameList * const ports)
{
    if (!verbose_)
        return true;
    printf("add primitive: %s %s (", type, name);
    MdtNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it)
        printf("%s ", *it);
    printf(")\n");
    return true;
}

bool MdtFile::addInstance(const char * const type
    , const char * const name
    , MdtNameList * const ports)
{
    if (!verbose_)
        return true;
    printf("add instance: %s %s (", type, name);
    MdtNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it)
        printf("%s ", *it);
    printf(")\n");
    return true;
}

bool MdtFile::addInstance(const char * const type
    , const char * const name
    , MdtPairList * const pairs)
{
    if (!verbose_)
        return true;
    printf("add instance: %s %s (", type, name);
    MdtPairList::iterator it = pairs->begin();
    for ( ; it != pairs->end(); ++it)
        printf(".%s(%s) ", (*it).first, (*it).second);
    printf(")\n");
    return true;
}

