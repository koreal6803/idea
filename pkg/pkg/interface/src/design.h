// **************************************************************************
// File       [ design.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/08/16 created ]
// **************************************************************************

#ifndef __INTF_DESIGN_H__
#define __INTF_DESIGN_H__

#include "primitive.h"
#include "occ.h"

namespace IntfNs {

class Design {
public:
    Design();
    ~Design();

    void setModules(ModuleVec mods);
    void setModels(ModuleVec models);
    void setPmts(ModuleVec pmts);
    void addModules(ModuleVec mods);
    void addModels(ModuleVec models);
    void addPmts(ModuleVec pmts);
    void setTop(const size_t &i);
    void setTop(const char * const name);

    size_t nModules() const;
    size_t nModels() const;
    size_t nPmts() const;
    Module *getModule(const char * const name) const;
    Module *getModule(const size_t &i) const;
    Module *getModel(const size_t &i) const;
    Module *getTop() const;
    Pmt *getPmt(const size_t &i) const;
    Occ *getOcc() const;

    bool linkModules();
    bool buildOcc();

protected:
    ModuleVec mods_;
    ModuleVec models_;
    ModuleVec pmts_;
    ModuleMap nameToMod_;
    Module    *top_;
    Occ       *occ_;
};

inline Design::Design() : top_(NULL), occ_(NULL) {}

inline Design::~Design() {}

inline void Design::setModules(ModuleVec mods) {
    for (size_t i = 0; i < mods_.size(); ++i)
        nameToMod_.erase(mods_[i]->getName());
    mods_ = mods;
    top_ = NULL;
    for (size_t i = 0; i < mods_.size(); ++i)
        nameToMod_[mods_[i]->getName()] = mods_[i];
    if (mods_.size() > 0)
        top_ = mods_[mods_.size() - 1];
}

inline void Design::setModels(ModuleVec models) {
    for (size_t i = 0; i < models_.size(); ++i)
        nameToMod_.erase(models_[i]->getName());
    models_ = models;
    for (size_t i = 0; i < models_.size(); ++i)
        nameToMod_[models_[i]->getName()] = models_[i];
}

inline void Design::setPmts(ModuleVec pmts) {
    for (size_t i = 0; i < pmts_.size(); ++i)
        nameToMod_.erase(pmts_[i]->getName());
    pmts_ = pmts;
    for (size_t i = 0; i < pmts_.size(); ++i)
        nameToMod_[pmts_[i]->getName()] = pmts_[i];
}

inline void Design::addModules(ModuleVec mods) {
    mods_.reserve(mods_.size() + distance(mods.begin(), mods.end()));
    mods_.insert(mods_.end(), mods.begin(), mods.end());
    for (size_t i = 0; i < mods.size(); ++i)
        nameToMod_[mods[i]->getName()] = mods[i];
}

inline void Design::addModels(ModuleVec models) {
    models_.reserve(models_.size() + distance(models.begin(), models.end()));
    models_.insert(models_.end(), models.begin(), models.end());
    for (size_t i = 0; i < models.size(); ++i)
        nameToMod_[models[i]->getName()] = models[i];
}

inline void Design::addPmts(ModuleVec pmts) {
    pmts_.reserve(pmts_.size() + distance(pmts.begin(), pmts.end()));
    pmts_.insert(pmts_.end(), pmts.begin(), pmts.end());
    for (size_t i = 0; i < pmts.size(); ++i)
        nameToMod_[pmts[i]->getName()] = pmts[i];
}

inline void Design::setTop(const size_t &i) {
    top_ = mods_[i];
}

inline void Design::setTop(const char * const name) {
    ModuleMap::iterator it = nameToMod_.find(name);
    if (it != nameToMod_.end())
        top_ = it->second;
}

inline size_t Design::nModules() const {
    return mods_.size();
}

inline size_t Design::nModels() const {
    return models_.size();
}

inline size_t Design::nPmts() const {
    return pmts_.size();
}

inline Module *Design::getModule(const char * const name) const {
    ModuleMap::const_iterator it = nameToMod_.find(name);
    return it == nameToMod_.end() ? NULL : it->second;
}

inline Module *Design::getModule(const size_t &i) const {
    return mods_[i];
}

inline Module *Design::getModel(const size_t &i) const {
    return models_[i];
}

inline Module *Design::getTop() const {
    return top_;
}

inline Pmt *Design::getPmt(const size_t &i) const {
    return (Pmt *)pmts_[i];
}

inline Occ *Design::getOcc() const {
    return occ_;
}

};

#endif


