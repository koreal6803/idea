// **************************************************************************
// File       [ vlog_file.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/06/24 created ]
// **************************************************************************

#include <cstdio>

#include "vlog_file.h"

using namespace std;
using namespace IntfNs;

extern int vlog_fileparse(void *);
extern FILE *vlog_filein;

bool VlogFile::read(const char * const fname, const bool &verbose) {
    verbose_ = verbose;
    vlog_filein = fopen(fname, "r");
    if (!vlog_filein) {
        fprintf(stderr, "**ERROR VlogFile::read(): cannot open netlist file");
        fprintf(stderr, "`%s'\n", fname);
        return false;
    }

    int res = vlog_fileparse(this);
    if (res != 0) {
        fprintf(stderr, "**ERROR VlogFile::read(): wrong input format\n");
        fclose(vlog_filein);
        return false;
    }
    fclose(vlog_filein);

    return true;
}


bool VlogFile::addModule(const char * const name) {
    if (!verbose_)
        return true;
        printf("add module: %s\n", name);
    return true;
}

bool VlogFile::addPorts(VlogNameList * const ports) {
    if (!verbose_)
        return true;
    printf("add ports: ");
    VlogNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setInputNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set input nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setOutputNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set output nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setInoutNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set inout nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setWireNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set wire nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setRegNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set reg nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setSupplyLNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set supply low nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}

bool VlogFile::setSupplyHNets(VlogNameList * const nets) {
    if (!verbose_)
        return true;
    printf("set supply high nets: ");
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it)
        printf("%s ", *it);
    printf("\n");
    return true;
}


bool VlogFile::addInst(const char * const type
    , const char * const name
    , VlogNameList * const ports)
{
    if (!verbose_)
        return true;
    printf("add inst: %s %s (", type, name);
    VlogNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it)
        printf("%s ", *it);
    printf(")\n");
    return true;
}

bool VlogFile::addInst(const char * const type
    , const char * const name
    , VlogPairList * const pairs)
{
    if (!verbose_)
        return true;
    printf("add inst: %s %s (", type, name);
    VlogPairList::iterator it = pairs->begin();
    for ( ; it != pairs->end(); ++it)
        printf(".%s(%s) ", (*it).first, (*it).second);
    printf(")\n");
    return true;
}

bool VlogFile::addInst(const char * const type
    , VlogNameList * const strengths
    , const char * const name
    , VlogNameList * const ports)
{
    if (!verbose_)
        return true;
    printf("add inst: %s (", type);
    VlogNameList::iterator it = strengths->begin();
    for ( ; it != strengths->end(); ++it)
        printf("%s ", *it);
    printf(") %s(", name);
    for (it = ports->begin(); it != ports->end(); ++it)
        printf("%s ", *it);
    printf(")\n");
    return true;
}


bool VlogFile::addAssign(const char * const n1, const char * const n2) {
    if (!verbose_)
        return true;
    printf("add assign: %s = %s\n", n1, n2);
    return true;
}

