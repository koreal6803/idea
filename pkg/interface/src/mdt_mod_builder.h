// **************************************************************************
// File       [ mdt_mod_builder.h ]
// Author     [ littleshamoo ]
// Synopsis   [ read in technology library and build into netlist data
//              structure ]
// Date       [ 2011/03/18 created ]
// **************************************************************************

#ifndef _INTF_MDT_MOD_BUILDER_H_
#define _INTF_MDT_MOD_BUILDER_H_
#include <cstdlib>
#include <cstdio>

#include "primitive.h"
#include "mdt_file.h"

namespace IntfNs {

class MdtModBuilder : public MdtFile {
public:
                 MdtModBuilder();
    virtual      ~MdtModBuilder();

    virtual bool read(const char * const fname, const bool &verbose = false);

    virtual bool addModel(const char * const name);
    virtual bool addPorts(MdtNameList * const ports);
    virtual bool setInputNets(MdtNameList * const nets);
    virtual bool setOutputNets(MdtNameList * const nets);
    virtual bool setInoutNets(MdtNameList * const nets);
    virtual bool setInternNets(MdtNameList * const nets);
    virtual bool addPrimitive(const char * const type
        , const char * const name
        , MdtNameList * const ports);
    virtual bool addInstance(const char * const type
        , const char * const name
        , MdtNameList * const ports);
    virtual bool addInstance(const char * const type
        , const char * const name
        , MdtPairList * const pairs);

    virtual size_t nModels() const;
    virtual Module *getModel(const size_t &i) const;
    virtual ModuleVec getModels() const;

    virtual size_t nPmts() const;
    virtual Pmt *getPmt(const size_t &i) const;
    virtual ModuleVec getPmts() const;

protected:
    bool      addTermNets(MdtNameList * const nets);
    void      createPmts();
    Module    *cur_;
    ModuleVec models_;
    ModuleVec pmts_;
};

inline MdtModBuilder::MdtModBuilder() : cur_(NULL) {}

inline MdtModBuilder::~MdtModBuilder() {
}

inline size_t MdtModBuilder::nModels() const {
    return models_.size();
}

inline Module *MdtModBuilder::getModel(const size_t &i) const {
    return models_[i];
}

inline ModuleVec MdtModBuilder::getModels() const {
    return models_;
}

inline size_t MdtModBuilder::nPmts() const {
    return pmts_.size();
}

inline Pmt *MdtModBuilder::getPmt(const size_t &i) const {
    return (Pmt *)pmts_[i];
}

inline ModuleVec MdtModBuilder::getPmts() const {
    return pmts_;
}

};

#endif

