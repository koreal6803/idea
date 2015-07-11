// **************************************************************************
// File       [ mdt_mod_builder.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/06/25 created ]
// **************************************************************************

#include "mdt_mod_builder.h"

using namespace std;
using namespace IntfNs;

bool MdtModBuilder::read(const char * const fname, const bool &verbose) {
    createPmts();
    return MdtFile::read(fname, verbose);
}

//{{{ bool MdtModBuilder::addModel()
bool MdtModBuilder::addModel(const char * const name) {
    cur_ = new Module(name);
    cur_->setIsmodel(true);
    models_.push_back(cur_);
    return true;
} //}}}
//{{{ bool MdtModBuilder::addPorts()
bool MdtModBuilder::addPorts(MdtNameList * const ports) {
    size_t pos = 0;
    MdtNameList::iterator it = ports->begin();
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
} //}}}
//{{{ bool MdtModBuilder::setInputNets()
bool MdtModBuilder::setInputNets(MdtNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::INPUT);
    }
    return true;
} //}}}
//{{{ bool MdtModBuilder::setOutputNets()
bool MdtModBuilder::setOutputNets(MdtNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::OUTPUT);
    }
    return true;
} //}}}
//{{{ bool MdtModBuilder::setInoutNets()
bool MdtModBuilder::setInoutNets(MdtNameList * const nets) {
    if (!addTermNets(nets))
        return false;
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        term->setType(ModTerm::INOUT);
    }
    return true;
} //}}}
//{{{ bool MdtModBuilder::setInternNets()
bool MdtModBuilder::setInternNets(MdtNameList * const nets) {
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        if (!cur_->getModNet(*it)) {
            ModNet *net = new ModNet(*it);
            net->setModule(cur_);
            cur_->addModNet(net);
        }
    }
    return true;
} //}}}
//{{{ bool MdtModBuilder::addPrimitive()
bool MdtModBuilder::addPrimitive(const char * const type
    , const char * const name
    , MdtNameList * const ports)
{
    const char *pname = name;
    char tmp[256];
    if (!strcmp(name, "")) {
        sprintf(tmp, "PMT_%zu", cur_->nModInsts());
        pname = tmp;
    }
    if (cur_->getModInst(pname)) {
        if (verbose_) {
            fprintf(stderr, "**ERROR VlogModBuilder::addPrimitive(): inst ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), pname);
            fprintf(stderr, "already exists\n");
        }
        return false;
    }

    // find primitive
    Module *pmt = NULL;
    for (size_t i = 0; i < pmts_.size(); ++i) {
        if (!strcmp(type, pmts_[i]->getName())) {
            pmt = pmts_[i];
            break;
        }
    }
    if (!pmt) {
        fprintf(stderr, "**ERROR MdtModBuilder::addPrimitive(): ");
        fprintf(stderr, "invalid primitive `%s/%s'\n", cur_->getName(), tmp);
        return false;
    }

    ModInst *inst = new ModInst(pname);
    inst->setTop(cur_);
    inst->setModName(tmp);
    inst->setModule(pmt);
    cur_->addModInst(inst);

    size_t count = 0;
    MdtNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it, ++count) {
        ModInstTerm *term = new ModInstTerm;
        term->setModInst(inst);
        term->setPos(count);
        term->setUsePos(true);
        inst->addModInstTerm(term);

        // terminal not connected to net
        if (!strcmp((*it), ""))
            continue;

        ModNet *net = cur_->getModNet(*it);
        if (!net) {
            if (verbose_) {
                fprintf(stderr, "**WARN MdtModBuilder::addPrimitive(): net ");
                fprintf(stderr, "`%s/%s' ", cur_->getName(), *it);
                fprintf(stderr, "set as wire\n");
            }
            net = new ModNet(*it);
            net->setModule(cur_);
            cur_->addModNet(net);
        }

        // connect net and terminal
        term->setModNet(net);
        net->addModInstTerm(term);
    }

    return true;
} //}}}
//{{{ bool MdtModBuilder::addInstance()
bool MdtModBuilder::addInstance(const char * const type
    , const char * const name
    , MdtNameList * const ports)
{
    const char *pname = name;
    char tmp[256];
    if (!strcmp(name, "")) {
        sprintf(tmp, "INST_%zu", cur_->nModInsts());
        pname = tmp;
    }
    if (cur_->getModInst(pname)) {
        if (verbose_) {
            fprintf(stderr, "**ERROR VlogModBuilder::addInstance(): inst ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), pname);
            fprintf(stderr, "already exists\n");
        }
        return false;
    }

    ModInst *inst = new ModInst(pname);
    inst->setTop(cur_);
    inst->setModName(type);
    cur_->addModInst(inst);

    size_t count = 0;
    MdtNameList::iterator it = ports->begin();
    for ( ; it != ports->end(); ++it, ++count) {
        ModInstTerm *term = new ModInstTerm;
        term->setModInst(inst);
        term->setPos(count);
        term->setUsePos(true);
        inst->addModInstTerm(term);

        // terminal not connected to net
        if (!strcmp((*it), ""))
            continue;

        ModNet *net = cur_->getModNet(*it);
        if (!net) {
            if (verbose_) {
                fprintf(stderr, "**WARN MdtModBuilder::addPrimitive(): net ");
                fprintf(stderr, "`%s/%s' ", cur_->getName(), *it);
                fprintf(stderr, "set as wire\n");
            }
            net = new ModNet(*it);
            net->setModule(cur_);
            cur_->addModNet(net);
        }

        // connect net and terminal
        term->setModNet(net);
        net->addModInstTerm(term);
    }

    return true;
} //}}}
//{{{ bool MdtModBuilder::addInstance()
bool MdtModBuilder::addInstance(const char * const type
    , const char * const name
    , MdtPairList * const pairs)
{
    const char *pname = name;
    char tmp[256];
    if (!strcmp(name, "")) {
        sprintf(tmp, "INST_%zu", cur_->nModInsts());
        pname = tmp;
    }
    if (cur_->getModInst(pname)) {
        if (verbose_) {
            fprintf(stderr, "**ERROR VlogModBuilder::addInstance(): inst ");
            fprintf(stderr, "`%s/%s' ", cur_->getName(), pname);
            fprintf(stderr, "already exists\n");
        }
        return false;
    }

    ModInst *inst = new ModInst(pname);
    inst->setTop(cur_);
    inst->setModName(type);
    cur_->addModInst(inst);

    MdtPairList::iterator it = pairs->begin();
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
} //}}}
//{{{ bool MdtModBuilder::addTermNets()
bool MdtModBuilder::addTermNets(MdtNameList * const nets) {
    MdtNameList::iterator it = nets->begin();
    for ( ; it != nets->end(); ++it) {
        ModTerm *term = cur_->getModTerm(*it);
        if (!term) {
            if (verbose_) {
                fprintf(stderr, "**ERROR MdtModBuilder::addTermNets(): ");
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
} //}}}
//{{{ void MdtModBuilder::createPmts()
void MdtModBuilder::createPmts() {
    pmts_.push_back(new PmtAnd("_and"));
    pmts_.push_back(new PmtNand("_nand"));
    pmts_.push_back(new PmtOr("_or"));
    pmts_.push_back(new PmtNor("_nor"));
    pmts_.push_back(new PmtInv("_inv"));
    pmts_.push_back(new PmtBuf("_buf"));
    pmts_.push_back(new PmtBufz("_bufz"));
    pmts_.push_back(new PmtXor("_xor"));
    pmts_.push_back(new PmtXnor("_xnor"));
    pmts_.push_back(new PmtTsl("_tsl"));
    pmts_.push_back(new PmtTsli("_tsli"));
    pmts_.push_back(new PmtTsh("_tsh"));
    pmts_.push_back(new PmtTshi("_tshi"));
    pmts_.push_back(new PmtMux("_mux"));
    pmts_.push_back(new PmtDff("_dff"));
    pmts_.push_back(new PmtDlat("_dlat"));
    pmts_.push_back(new PmtDelay("_delay"));
    pmts_.push_back(new PmtInvf("_invf"));
    pmts_.push_back(new PmtWire("_wire"));
    pmts_.push_back(new PmtPull("_pull"));
    pmts_.push_back(new PmtTie1("_tie1"));
    pmts_.push_back(new PmtTie0("_tie0"));
    pmts_.push_back(new PmtTieX("_tiex"));
    pmts_.push_back(new PmtTieZ("_tiez"));
    pmts_.push_back(new PmtUndef("_undef"));
    pmts_.push_back(new PmtNmos("_nmos"));
    pmts_.push_back(new PmtPmos("_pmos"));
    pmts_.push_back(new PmtRnmos("_rnmos"));
    pmts_.push_back(new PmtRpmos("_rpmos"));
    pmts_.push_back(new PmtNmosf("_nmosf"));
    pmts_.push_back(new PmtPmosf("_pmosf"));
    pmts_.push_back(new PmtCmos1("_cmos1"));
    pmts_.push_back(new PmtCmos2("_cmos2"));
    pmts_.push_back(new PmtRcmos1("_rcmos1"));
    pmts_.push_back(new PmtRcmos2("_rcmos2"));
    pmts_.push_back(new PmtCmos1f("_cmos1f"));
    pmts_.push_back(new PmtCmos2f("_cmos2f"));
} //}}}

