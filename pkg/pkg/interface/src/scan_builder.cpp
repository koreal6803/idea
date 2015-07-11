// **************************************************************************
// File       [ scan_builder.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/12/28 created ]
// **************************************************************************

#include <cstdio>

#include "scan_builder.h"

using namespace std;
using namespace IntfNs;

bool ScanBuilder::addGroup(const size_t &nchain) {
    group_ = new ScanGroup;
    return true;
}

bool ScanBuilder::addChain(const char * const name, const char * const clk,
                           const char * const si, const char * const so,
                           const char * const se, const int &len) {
    ScanChain *chain = new ScanChain(name, clk, si, so, se);
    group_->addScanChain(chain);
    return true;
}

bool ScanBuilder::addCell(const char * const name, const char * const in,
                          const char * const out, const char * const inv) {
    if (strlen(inv) != 4)
        return false;

    bool siInv = false;
    bool soInv = false;
    bool ciInv = false;
    bool coInv = false;
    if (inv[0] == 'T')
        siInv = true;
    if (inv[1] == 'T')
        soInv = true;
    if (inv[2] == 'T')
        ciInv = true;
    if (inv[3] == 'T')
        coInv = true;

    ScanCell *cell = new ScanCell(name, in, out, siInv, soInv, ciInv, coInv);
    group_->getScanChain(group_->nScanChains() - 1)->addScanCell(cell);
    return true;
}

