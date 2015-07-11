// **************************************************************************
// File       [ vlog_mod_builder.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/03/01 created ]
// **************************************************************************

#include "vlog_mod_builder.h"

using namespace std;
using namespace IntfNs;

bool VlogModBuilder::addModule(const char * const name) {
    cur_ = new Module(name);
    mods_.push_back(cur_);
    return true;
}

bool VlogModBuilder::addPorts(VlogNameList * const ports) {
    size_t pos = 0;
    VlogNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it, ++pos) {
        if (cur_->getModTerm((*it))) {
            if (verbose_) {
                fprintf(stderr, "**ERROR VlogModBuilder::addPorts(): port ");
                fprintf(stderr, "`%s/%s' ", cur_->getName(), (*it));
                fprintf(stderr, "already exists\n");
            }
            return false;
        }
        ModTerm *term = new ModTerm((*it));
        term->setPos(pos);
        term->setModule(cur_);
        cur_->addModTerm(term);
    }
    return true;
}

bool VlogModBuilder::setInputNets(VlogNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::INPUT);
    }
    return true;
}

bool VlogModBuilder::setOutputNets(VlogNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::OUTPUT);
    }
    return true;
}

bool VlogModBuilder::setInoutNets(VlogNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::INOUT);
    }
    return true;
}

bool VlogModBuilder::setWireNets(VlogNameList * const nets) {
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        if (!cur_->getModNet(*it)) {
            ModNet *net = new ModNet(*it);
            net->setModule(cur_);
            cur_->addModNet(net);
        }
    }
    return true;
}

bool VlogModBuilder::addInst(const char * const type
    , const char * const name
    , VlogPairList * const pairs)
{
    const char *iname = name;
    char tmp[256];
    if (!strcmp(name, "")) {
        sprintf(tmp, "INST_%zu", cur_->nModInsts());
        iname = tmp;
    }
    if (cur_->getModInst(iname)) {
        if (verbose_) {
            fprintf(stderr, "**ERROR VlogModBuilder::addInst(): inst ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), iname);
            fprintf(stderr, "already exists\n");
        }
        return false;
    }

    ModInst *inst = new ModInst(iname);
    inst->setTop(cur_);
    inst->setModName(type);
    cur_->addModInst(inst);

    VlogPairList::iterator it = pairs->begin();
    for ( ; it != pairs->end(); ++it) {
        ModInstTerm *term = new ModInstTerm;
        term->setName((*it).first);
        term->setModInst(inst);
        term->setUsePos(false);
        inst->addModInstTerm(term);

        // terminal not connected to net
        if (!strcmp((*it).second, ""))
            continue;

        // create net if not exist
        ModNet *net = cur_->getModNet((*it).second);
        if (!net) {
            if (verbose_) {
                fprintf(stderr, "** WARN VlogModBuilder::addInst(): net ");
                fprintf(stderr, "`%s/%s' ", cur_->getName(), (*it).second);
                fprintf(stderr, "set as wire\n");
            }
            net = new ModNet((*it).second);
            net->setModule(cur_);
            cur_->addModNet(net);
        }

        // connect net and terminal
        term->setModNet(net);
        net->addModInstTerm(term);
    }
    return true;
}


bool VlogModBuilder::addAssign(const char * const n1, const char * const n2) {
    ModNet *net1 = cur_->getModNet(n1);
    if (!net1) {
        if (verbose_) {
            fprintf(stderr, "**WARN VlogModBuilder::addAssign(): net ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), n1);
            fprintf(stderr, "set as wire\n");
        }
        net1 = new ModNet(n1);
        net1->setModule(cur_);
        cur_->addModNet(net1);
    }

    ModNet *net2 = cur_->getModNet(n2);
    if (!net2) {
        if (verbose_) {
            fprintf(stderr, "**WARN VlogModBuilder::addAssign(): net ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), n2);
            fprintf(stderr, "set as wire\n");
        }
        net2 = new ModNet(n2);
        net2->setModule(cur_);
        cur_->addModNet(net2);
    }

    net1->addEqv(net2);
    net2->addEqv(net1);

    return true;
}

bool VlogModBuilder::addTermNets(VlogNameList * const nets) {
    VlogNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        if (!term) {
            if (verbose_) {
                fprintf(stderr, "**ERROR VlogModBuilder::addTermNets(): ");
                fprintf(stderr, "port `%s/%s' ", cur_->getName(), *it);
                fprintf(stderr, "does not exist\n");
            }
            return false;
        }
        ModNet *net = cur_->getModNet(*it);
        if (!net) {
            net = new ModNet(*it);
            net->setModule(cur_);
            cur_->addModNet(net);
        }
        net->addModTerm(term);
        term->setModNet(net);
    }
    return true;
}

bool VlogModBuilder::setRegNets(VlogNameList * const nets) {
    return false;
}

bool VlogModBuilder::setSupplyLNets(VlogNameList * const nets) {
    return false;
}

bool VlogModBuilder::setSupplyHNets(VlogNameList * const nets) {
    return false;
}
bool VlogModBuilder::addInst(const char * const type
    , const char * const name
    , VlogNameList * const ports)
{
    return false;
}

bool VlogModBuilder::addInst(const char * const type
    , VlogNameList * const strengths
    , const char * const name
    , VlogNameList * const ports)
{
    return false;
}


