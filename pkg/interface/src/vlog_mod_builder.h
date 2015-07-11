// **************************************************************************
// File       [ vlog_mod_builder.h ]
// Author     [ littleshamoo ]
// Synopsis   [ read in netlist and build into netlist data structure ]
// Date       [ ]
// **************************************************************************

#ifndef __INTF_VLOG_MOD_BUILDER_H__
#define __INTF_VLOG_MOD_BUILDER_H__

#include <cstdlib>
#include <cstdio>
#include "module.h"
#include "vlog_file.h"

namespace IntfNs {

class VlogModBuilder : public VlogFile {
public:
                 VlogModBuilder();
    virtual      ~VlogModBuilder();

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

    virtual size_t nModules() const;
    virtual Module *getModule(const size_t &i) const;
    virtual ModuleVec getModules() const;

protected:
    bool      addTermNets(VlogNameList * const nets);
    Module    *cur_;
    ModuleVec mods_;
};


inline VlogModBuilder::VlogModBuilder() : cur_(NULL) {}

inline VlogModBuilder::~VlogModBuilder() {
}

inline size_t VlogModBuilder::nModules() const {
    return mods_.size();
}

inline Module *VlogModBuilder::getModule(const size_t &i) const {
    return mods_[i];
}

inline ModuleVec VlogModBuilder::getModules() const {
    return mods_;
}

};

#endif

