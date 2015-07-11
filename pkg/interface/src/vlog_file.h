// **************************************************************************
// File       [ vlog_file.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/06/24 created ]
// **************************************************************************

#ifndef __INTF_VLOG_FILE_H__
#define __INTF_VLOG_FILE_H__

#include <list>

namespace IntfNs {

enum VlogNetType {
    VLOG_NET_INPUT = 0
    , VLOG_NET_OUTPUT
    , VLOG_NET_INOUT
    , VLOG_NET_WIRE
    , VLOG_NET_REG
    , VLOG_NET_SUPPLY_L
    , VLOG_NET_SUPPLY_H
};

typedef std::list<char *>                     VlogNameList;
typedef std::list<std::pair<char *, char *> > VlogPairList;

class VlogFile {
public:
                 VlogFile();
    virtual      ~VlogFile();

    virtual bool read(const char * const fname, const bool &verbose = false);

    virtual bool addModule(const char * const name);
    virtual bool addPorts(VlogNameList * const ports);
    virtual bool setInputNets(VlogNameList * const nets);
    virtual bool setOutputNets(VlogNameList * const nets);
    virtual bool setInoutNets(VlogNameList * const nets);
    virtual bool setWireNets(VlogNameList * const nets);
    virtual bool setRegNets(VlogNameList * const nets);
    virtual bool setSupplyLNets(VlogNameList * const nets);
    virtual bool setSupplyHNets(VlogNameList * const nets);

    virtual bool addInst(const char * const type, const char * const name,
                         VlogNameList * const ports);
    virtual bool addInst(const char * const type, const char * const name,
                         VlogPairList * const pairs);
    virtual bool addInst(const char * const type,
                         VlogNameList * const strengths,
                         const char * const name,
                         VlogNameList * const ports);
    virtual bool addAssign(const char * const n1, const char * const n2);

protected:
    bool verbose_;
};

inline VlogFile::VlogFile() {
    verbose_ = false;
}

inline VlogFile::~VlogFile() {}

};

#endif

