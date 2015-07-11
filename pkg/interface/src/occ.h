// **************************************************************************
// File       [ occ.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/09/10 created ]
// **************************************************************************

#ifndef __INTF_OCC_H__
#define __INTF_OCC_H__

#include <stack>

#include "module.h"

namespace IntfNs {

class Occ;

typedef std::vector<Occ *>        OccVec;
typedef std::map<ChstrKey, Occ *> OccMap;

class Occ {
public:
    Occ(const size_t &mapBegin = 10);
    ~Occ();

    // children
    void addChild(Occ * const occ);
    size_t nChildren() const;
    Occ *getChild(const size_t &i) const;
    Occ *getChild(const char * const name) const;

    // parent
    void setParent(Occ * const occ);
    Occ *getParent() const;

    // get corresponding module instance
    void setModInst(ModInst * const modInst);
    ModInst *getModInst() const;

protected:
    size_t  mapBegin_;
    OccVec  occs_;
    OccMap  nameToOcc_;
    ModInst *modInst_;
    Occ     *parent_;
};

inline Occ::Occ(const size_t &mapBegin)
    : mapBegin_(mapBegin)
    , modInst_(NULL)
    , parent_(NULL) {}

inline Occ::~Occ() {}

inline void Occ::addChild(Occ * const occ) {
    occs_.push_back(occ);
    if (occs_.size() == mapBegin_) {
        for (size_t i = 0; i < occs_.size(); ++i)
            if (occs_[i]->modInst_)
                nameToOcc_[occs_[i]->modInst_->getName()] = occs_[i];
    }
    else if (occs_.size() > mapBegin_)
        if (occs_[occs_.size() - 1]->modInst_)
            nameToOcc_[occs_[occs_.size() - 1]->modInst_->getName()] = occ;
}

inline size_t Occ::nChildren() const {
    return occs_.size();
}

inline Occ *Occ::getChild(const size_t &i) const {
    return occs_[i];
}

inline Occ *Occ::getChild(const char * const name) const {
    if (occs_.size() < mapBegin_) {
        for (size_t i = 0; i < occs_.size(); ++i)
            if (occs_[i]->modInst_
                && strcmp(occs_[i]->modInst_->getName(), name) == 0)
                return occs_[i];
    }
    else {
        OccMap::const_iterator it = nameToOcc_.find(name);
        if (it != nameToOcc_.end())
            return it->second;
    }
    return NULL;
}

inline void Occ::setParent(Occ * const occ) {
    parent_ = occ;
}

inline Occ *Occ::getParent() const {
    return parent_;
}


inline void Occ::setModInst(ModInst * const modInst) {
    modInst_ = modInst;
}

inline ModInst *Occ::getModInst() const {
    return modInst_;
}

};

#endif



