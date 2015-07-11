// **************************************************************************
// File       [ design.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2012/08/17 created ]
// **************************************************************************

#include <queue>

#include "design.h"

using namespace std;
using namespace IntfNs;

bool Design::linkModules() {
    for (size_t i = 0; i < mods_.size(); ++i) {
        Module *mod = mods_[i];
        for (size_t j = 0; j < mod->nModInsts(); ++j) {
            ModInst *inst = mod->getModInst(j);
            Module *link = getModule(inst->getModName());
            if (!link)
                inst->setIsblackbox(true);
            else
                inst->setModule(link);
        }
    }
    for (size_t i = 0 ; i < models_.size(); ++i) {
        Module *model = models_[i];
        for (size_t j = 0; j < model->nModInsts(); ++j) {
            ModInst *inst = model->getModInst(j);
            if (inst->getModule())
                continue;
            Module *link = getModule(inst->getModName());
            if (!link)
                inst->setIsblackbox(true);
            else
                inst->setModule(link);
        }
    }
    return true;
}

bool Design::buildOcc() {
    delete occ_;
    if (!top_)
        return false;
    occ_ = new Occ;
    queue<Occ *> que;
    que.push(occ_);

    while (!que.empty()) {
        Occ *occ = que.front();
        que.pop();
        Module *module = NULL;
        if (!occ->getModInst()) // root don't have module instance
            module = top_;
        else
            module = occ->getModInst()->getModule();
        for (size_t i = 0; i < module->nModInsts(); ++i) {
            Occ *child = new Occ;
            child->setModInst(module->getModInst(i));
            occ->addChild(child);
            child->setParent(occ);
            que.push(child);
        }
    }

    return true;
}

