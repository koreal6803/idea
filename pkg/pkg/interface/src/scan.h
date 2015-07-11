// **************************************************************************
// File       [ scan.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/12/28 created ]
// **************************************************************************

#ifndef __INTF_SCAN_H__
#define __INTF_SCAN_H__

#include <cstring>
#include <vector>

namespace IntfNs {

class ScanChain;
class ScanCell;

class ScanGroup {
public:
    ScanGroup();
    ~ScanGroup();

    void addScanChain(ScanChain * const chain);
    size_t nScanChains() const;
    ScanChain *getScanChain(const size_t &i) const;

protected:
    std::vector<ScanChain *> chains_;
};

class ScanChain {
public:
    ScanChain(const char * const name
        , const char * const clk
        , const char * const si
        , const char * const so
        , const char * const se
    );
    ~ScanChain();

    void addScanCell(ScanCell * const cell);
    const char *getName() const;
    const char *getClk() const;
    const char *getSi() const;
    const char *getSo() const;
    const char *getSe() const;
    size_t nScanCells() const;
    ScanCell *getScanCell(const size_t &i) const;

protected:
    char *name_;
    char *clk_;
    char *si_;
    char *so_;
    char *se_;
    std::vector<ScanCell *> cells_; // from scan out to scan in
};


class ScanCell {
public:
    ScanCell(const char * const name
        , const char * const in
        , const char * const out
        , bool siInv
        , bool soInv
        , bool ciInv
        , bool coInv
    );
    ~ScanCell();

    const char *getName() const;
    const char *getIn() const;
    const char *getOut() const;
    bool getSiInv() const;
    bool getSoInv() const;
    bool getCiInv() const;
    bool getCoInv() const;

protected:
    char *name_;
    char *in_;
    char *out_;
    bool siInv_;
    bool soInv_;
    bool ciInv_;
    bool coInv_;
};


// inline methods
inline ScanGroup::ScanGroup() {}

inline ScanGroup::~ScanGroup() {
    for (size_t i = 0; i < chains_.size(); ++i)
        delete chains_[i];
}

inline void ScanGroup::addScanChain(ScanChain *const chain) {
    chains_.push_back(chain);
}

inline size_t ScanGroup::nScanChains() const {
    return chains_.size();
}

inline ScanChain *ScanGroup::getScanChain(const size_t &i) const {
    return chains_[i];
}

inline ScanChain::ScanChain(const char * const name
    , const char * const clk
    , const char * const si
    , const char * const so
    , const char * const se)
    : name_(strdup(name))
    , clk_(strdup(clk))
    , si_(strdup(si))
    , so_(strdup(so))
    , se_(strdup(se)) {}


inline ScanChain::~ScanChain() {
    delete [] name_;
    delete [] clk_;
    delete [] si_;
    delete [] so_;
    delete [] se_;
    for (size_t i = 0; i < cells_.size(); ++i)
        delete cells_[i];
}

inline void ScanChain::addScanCell(ScanCell * const cell) {
    cells_.push_back(cell);
}

inline const char *ScanChain::getName() const {
    return name_;
}

inline const char *ScanChain::getClk() const {
    return clk_;
}

inline const char *ScanChain::getSi() const {
    return si_;
}

inline const char *ScanChain::getSo() const {
    return so_;
}

inline const char *ScanChain::getSe() const {
    return se_;
}

inline size_t ScanChain::nScanCells() const {
    return cells_.size();
}

inline ScanCell *ScanChain::getScanCell(const size_t &i) const {
    return cells_[i];
}

inline ScanCell::ScanCell(const char * const name
    , const char * const in
    , const char * const out
    , bool siInv
    , bool soInv
    , bool ciInv
    , bool coInv)
    : name_(strdup(name))
    , in_(strdup(in))
    , out_(strdup(out))
    , siInv_(siInv)
    , soInv_(soInv)
    , ciInv_(ciInv)
    , coInv_(coInv) {}

inline ScanCell::~ScanCell() {
    delete [] name_;
    delete [] in_;
    delete [] out_;
}

inline const char *ScanCell::getName() const {
    return name_;
}

inline const char *ScanCell::getIn() const {
    return in_;
}

inline const char *ScanCell::getOut() const {
    return out_;
}

inline bool ScanCell::getSiInv() const {
    return siInv_;
}

inline bool ScanCell::getSoInv() const {
    return soInv_;
}

inline bool ScanCell::getCiInv() const {
    return ciInv_;
}

inline bool ScanCell::getCoInv() const {
    return coInv_;
}


};

#endif


