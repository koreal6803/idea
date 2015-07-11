// **************************************************************************
// File       [ scan_builder.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/12/28 created ]
// **************************************************************************

#ifndef __INTF_SCAN_BUILDER_H__
#define __INTF_SCAN_BUILDER_H__

#include "scan_file.h"
#include "scan.h"

namespace IntfNs {

class ScanBuilder : public ScanFile {
public:
                 ScanBuilder();
    virtual      ~ScanBuilder();

    virtual bool addGroup(const size_t &nchains);
    virtual bool addChain(const char * const name, const char * const clk,
                          const char * const si, const char * const so,
                          const char * const se, const int &len);
    virtual bool addCell(const char * const name, const char * const in,
                         const char * const out, const char * const inv);

    virtual ScanGroup *getScanGroup() const;

protected:
    ScanGroup *group_;
};

inline ScanBuilder::ScanBuilder() : group_(NULL) {}

inline ScanBuilder::~ScanBuilder() {}

inline ScanGroup *ScanBuilder::getScanGroup() const {
    return group_;
}

};

#endif


