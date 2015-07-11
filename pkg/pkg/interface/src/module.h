// **************************************************************************
// File       [ module.h ]
// Author     [ littleshamoo ]
// Synopsis   [ define cell structure ]
// Date       [ 2011/03/23 created ]
// **************************************************************************

#ifndef __INTF_MODULE_H__
#define __INTF_MODULE_H__

#include <set>
#include <vector>
#include <map>

#include "chstr_key.h"

namespace IntfNs {

class Module;
class ModInst;
class ModInstTerm;
class ModTerm;
class ModNet;

typedef std::vector<Module *>        ModuleVec;
typedef std::set<Module *>           ModuleSet;
typedef std::map<ChstrKey, Module *> ModuleMap;

typedef std::vector<ModTerm *>        ModTermVec;
typedef std::set<ModTerm *>           ModTermSet;
typedef std::map<ChstrKey, ModTerm *> ModTermMap;

typedef std::vector<ModInst *>        ModInstVec;
typedef std::set<ModInst *>           ModInstSet;
typedef std::map<ChstrKey, ModInst *> ModInstMap;

typedef std::vector<ModInstTerm *>        ModInstTermVec;
typedef std::set<ModInstTerm *>           ModInstTermSet;
typedef std::map<ChstrKey, ModInstTerm *> ModInstTermMap;

typedef std::vector<ModNet *>         ModNetVec;
typedef std::set<ModNet *>            ModNetSet;
typedef std::map<ChstrKey, ModNet *>  ModNetMap;

// {{{ class Module
class Module {
public:
            Module(const char * const name);
    virtual ~Module();

    const char *getName() const;
    bool ismodel() const;
    bool ispmt() const;

    // terminals
    size_t  nModTerms() const;
    ModTerm *getModTerm(const size_t &i) const;
    ModTerm *getModTerm(const char * const name) const;

    // internal instances
    size_t  nModInsts() const;
    ModInst *getModInst(const size_t &i) const;
    ModInst *getModInst(const char * const name) const;

    // internal nets
    size_t nModNets() const;
    ModNet *getModNet(const size_t &i) const;
    ModNet *getModNet(const char * const name) const;

    // for builder
    void setIsmodel(const bool &ismodel);
    void setIspmt(const bool &ispmt);
    bool addModInst(ModInst * const inst);
    bool addModTerm(ModTerm * const term);
    bool addModNet(ModNet * const net);

protected:
    char       *name_;
    ModTermVec modTerms_;
    ModTermMap nameToModTerm_;
    ModInstVec modInsts_;
    ModInstMap nameToModInst_;
    ModNetVec  modNets_;
    ModNetMap  nameToModNet_;
    bool       ismodel_;
    bool       ispmt_;
}; //}}}
//{{{ class ModInst
class ModInst {
public:
            ModInst(const char * const name);
    virtual ~ModInst();

    const char *getName() const;
    const char *getModName() const;
    bool isblackbox() const;

    Module *getTop() const;
    Module *getModule() const;

    // get instance terminals
    size_t      nModInstTerms() const;
    ModInstTerm *getModInstTerm(const size_t &i) const;
    ModInstTerm *getModInstTerm(const char * const name) const;

    // for builder
    void setIsblackbox(const bool &isblackbox);
    void setModule(Module * const mod);
    void setModName(const char * const name);
    void setTop(Module * const top);
    bool addModInstTerm(ModInstTerm * const modInstTerm);

protected:
    char           *name_;
    Module         *top_;
    Module         *mod_;
    char           *modName_;
    ModInstTermVec modInstTerms_;
    bool           isblackbox_;
}; //}}}
//{{{ class ModInstTerm
class ModInstTerm {
public:
            ModInstTerm();
    virtual ~ModInstTerm();

    const char *getName() const;
    size_t getPos() const;
    bool getUsePos() const;

    // get corrensponding module instance
    ModInst *getModInst() const;

    // get connected net
    ModNet *getModNet() const;

    // for builder
    void setModInst(ModInst *modInst);
    void setModNet(ModNet *modNet);
    void setName(const char * const name);
    void setPos(const size_t &pos);
    void setUsePos(const bool &usePos);

protected:
    char    *name_;
    size_t  pos_;
    bool    usePos_;
    ModInst *modInst_;
    ModNet  *modNet_;
    ModTerm *modTerm_;
}; //}}}
//{{{ class ModTerm
class ModTerm {
public:

    enum    Type { INPUT = 0, OUTPUT, INOUT };

            ModTerm(const char * const name);
    virtual ~ModTerm();

    const char *getName() const;
    Type getType() const;

    // get corresponding module
    Module *getModule() const;

    // get connected net
    ModNet *getModNet() const;

    // get terminal position
    size_t getPos() const;

    // for builder
    void setPos(const size_t &pos);
    void setType(const Type &type);
    void setModule(Module * const mod);
    void setModNet(ModNet * const modNet);

protected:
    char   *name_;
    Type   type_;
    Module *mod_;
    ModNet *modNet_;
    size_t pos_;
}; //}}}
//{{{ class ModNet
class ModNet {
public:
            ModNet(const char * const name);
    virtual ~ModNet();

    const char *getName() const;
    Module *getModule() const;

    // get terminals
    size_t  nModTerms() const;
    ModTerm *getModTerm(const size_t &i) const;
    ModTerm *getModTerm(const char * const name) const;

    // get instance terminals
    size_t nModInstTerms() const;
    ModInstTerm *getModInstTerm(const size_t &i) const;

    // get equivalent nets
    size_t nEqvs() const;
    ModNet *getEqv(const size_t &i) const;

    // for builder
    bool addEqv(ModNet * const modNet);
    bool addModTerm(ModTerm * const modTerm);
    bool addModInstTerm(ModInstTerm * const modInstTerm);
    void setModule(Module * const mod);

protected:
    char           *name_;
    Module         *mod_;
    ModNetVec      eqvs_;
    ModTermVec     modTerms_;
    ModInstTermVec modInstTerms_;
}; //}}}

//{{{ inline class Module
inline Module::Module(const char * const name)
    : name_(strdup(name))
    , ismodel_(false)
    , ispmt_(false) {}

inline Module::~Module() {
    delete [] name_;
}

inline const char *Module::getName() const {
    return name_;
}

inline bool Module::ismodel() const {
    return ismodel_;
}

inline bool Module::ispmt() const {
    return ispmt_;
}

inline size_t Module::nModTerms() const {
    return modTerms_.size();
}

inline ModTerm *Module::getModTerm(const size_t &i) const {
    return modTerms_[i];
}

inline ModTerm *Module::getModTerm(const char * const name) const {
    ModTermMap::const_iterator it = nameToModTerm_.find(name);
    return it == nameToModTerm_.end() ? NULL : it->second;
}

inline size_t Module::nModInsts() const {
    return modInsts_.size();
}

inline ModInst *Module::getModInst(const size_t &i) const {
    return modInsts_[i];
}

inline ModInst *Module::getModInst(const char * const name) const {
    ModInstMap::const_iterator it = nameToModInst_.find(name);
    return it == nameToModInst_.end() ? NULL : it->second;
}

inline size_t Module::nModNets() const {
    return modNets_.size();
}

inline ModNet *Module::getModNet(const size_t &i) const {
    return modNets_[i];
}

inline ModNet *Module::getModNet(const char * const name) const {
    ModNetMap::const_iterator it = nameToModNet_.find(name);
    return it == nameToModNet_.end() ? NULL : it->second;
}

inline void Module::setIsmodel(const bool &ismodel) {
    ismodel_ = ismodel;
}

inline void Module::setIspmt(const bool &ispmt) {
    ispmt_ = ispmt;
}

inline bool Module::addModInst(ModInst * const inst) {
    ModInstMap::iterator it = nameToModInst_.find(inst->getName());
    if (it != nameToModInst_.end())
        return false;
    nameToModInst_[inst->getName()] = inst;
    modInsts_.push_back(inst);
    return true;
}

inline bool Module::addModTerm(ModTerm * const term) {
    ModTermMap::iterator it = nameToModTerm_.find(term->getName());
    if (it != nameToModTerm_.end())
        return false;
    nameToModTerm_[term->getName()] = term;
    modTerms_.push_back(term);
    return true;
}

inline bool Module::addModNet(ModNet * const net) {
    ModNetMap::iterator it = nameToModNet_.find(net->getName());
    if (it != nameToModNet_.end())
        return false;
    nameToModNet_[net->getName()] = net;
    modNets_.push_back(net);
    return true;
}
//}}}
//{{{ inline class ModInst
inline ModInst::ModInst(const char * const name)
    : name_(strdup(name))
    , top_(NULL)
    , mod_(NULL)
    , modName_(NULL)
    , isblackbox_(false) {}

inline ModInst::~ModInst() {
    delete [] name_;
}

inline const char *ModInst::getName() const {
    return name_;
}

inline const char *ModInst::getModName() const {
    return modName_;
}

inline bool ModInst::isblackbox() const {
    return isblackbox_;
}

inline Module *ModInst::getModule() const {
    return mod_;
}

inline size_t ModInst::nModInstTerms() const {
    return modInstTerms_.size();
}

inline ModInstTerm *ModInst::getModInstTerm(const size_t &i) const {
    return modInstTerms_[i];
}

inline ModInstTerm *ModInst::getModInstTerm(const char * const name) const {
    for (size_t i = 0; i < modInstTerms_.size(); ++i)
        if (strcmp(name, modInstTerms_[i]->getName()) == 0)
            return modInstTerms_[i];
    return NULL;
}

inline void ModInst::setIsblackbox(const bool &isblackbox) {
    isblackbox_ = isblackbox;
}

inline void ModInst::setModule(Module * const mod) {
    mod_ = mod;
}

inline void ModInst::setTop(Module * const top) {
    top_ = top;
}

inline void ModInst::setModName(const char * const name) {
    delete [] modName_;
    modName_ = strdup(name);
}

inline bool ModInst::addModInstTerm(ModInstTerm * const modInstTerm) {
    modInstTerms_.push_back(modInstTerm);
    return true;
}
//}}}
//{{{ inline class ModInstTerm
inline ModInstTerm::ModInstTerm()
    : name_(NULL)
    , pos_(0)
    , usePos_(false)
    , modInst_(NULL)
    , modNet_(NULL)
    , modTerm_(NULL) {}

inline ModInstTerm::~ModInstTerm() {
    delete [] name_;
}

inline const char *ModInstTerm::getName() const {
    return name_;
}

inline size_t ModInstTerm::getPos() const {
    return pos_;
}

inline bool ModInstTerm::getUsePos() const {
    return usePos_;
}

inline ModInst *ModInstTerm::getModInst() const {
    return modInst_;
}

inline ModNet *ModInstTerm::getModNet() const {
    return modNet_;
}

inline void ModInstTerm::setModInst(ModInst *modInst) {
    modInst_ = modInst;
}

inline void ModInstTerm::setModNet(ModNet *modNet) {
    modNet_ = modNet;
}

inline void ModInstTerm::setName(const char * const name) {
    delete [] name_;
    name_ = strdup(name);
}

inline void ModInstTerm::setPos(const size_t &pos) {
    pos_ = pos;
}

inline void ModInstTerm::setUsePos(const bool &usePos) {
    usePos_ = usePos;
}
//}}}
//{{{ inline class ModTerm
inline ModTerm::ModTerm(const char * const name)
    : name_(strdup(name))
    , mod_(NULL)
    , modNet_(NULL)
    , pos_(0) {}

inline ModTerm::~ModTerm() {
    delete [] name_;
}

inline const char *ModTerm::getName() const {
    return name_;
}

inline ModTerm::Type ModTerm::getType() const {
    return type_;
}

inline Module *ModTerm::getModule() const {
    return mod_;
}

inline ModNet *ModTerm::getModNet() const {
    return modNet_;
}

inline size_t ModTerm::getPos() const {
    return pos_;
}

inline void ModTerm::setPos(const size_t &pos) {
    pos_ = pos;
}

inline void ModTerm::setType(const Type &type) {
    type_ = type;
}

inline void ModTerm::setModule(Module * const mod) {
    mod_ = mod;
}

inline void ModTerm::setModNet(ModNet * const modNet) {
    modNet_ = modNet;
}
//}}}
//{{{ inline class ModNet
inline ModNet::ModNet(const char * const name)
    : name_(strdup(name))
    , mod_(NULL) {}

inline ModNet::~ModNet() {
    delete [] name_;
}

inline const char *ModNet::getName() const {
    return name_;
}

inline Module *ModNet::getModule() const {
    return mod_;
}

inline size_t ModNet::nModTerms() const {
    return modTerms_.size();
}

inline ModTerm *ModNet::getModTerm(const size_t &i) const {
    return modTerms_[i];
}

inline ModTerm *ModNet::getModTerm(const char * const name) const {
    for (size_t i = 0; i < modTerms_.size(); ++i)
        if (strcmp(name, modTerms_[i]->getName()) == 0)
            return modTerms_[i];
    return NULL;
}

inline size_t ModNet::nModInstTerms() const {
    return modInstTerms_.size();
}

inline ModInstTerm *ModNet::getModInstTerm(const size_t &i) const {
    return modInstTerms_[i];
}

inline size_t ModNet::nEqvs() const {
    return eqvs_.size();
}

inline ModNet *ModNet::getEqv(const size_t &i) const {
    return eqvs_[i];
}

inline bool ModNet::addEqv(ModNet * const modNet) {
    eqvs_.push_back(modNet);
    return true;
}

inline bool ModNet::addModTerm(ModTerm * const modTerm) {
    modTerms_.push_back(modTerm);
    return true;
}

inline bool ModNet::addModInstTerm(ModInstTerm * const modInstTerm) {
    modInstTerms_.push_back(modInstTerm);
    return true;
}

inline void ModNet::setModule(Module * const mod) {
    mod_ = mod;
}
//}}}

};

#endif

