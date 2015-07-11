// **************************************************************************
// File       [ scan_file.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/12/22 created ]
// **************************************************************************

#ifndef __INTF_SCAN_FILE_H__
#define __INTF_SCAN_FILE_H__

namespace IntfNs {

class ScanFile {
public:
                 ScanFile();
    virtual      ~ScanFile();

    virtual bool read(const char * const fname, const bool &verbose = false);

    virtual bool addGroup(const size_t &nchains);
    virtual bool addChain(const char * const name, const char * const clk,
                          const char * const si, const char * const so,
                          const char * const se, const int &len);
    virtual bool addCell(const char * const name, const char * const in,
                         const char * const out, const char * const inv);


protected:
    bool verbose_;

    static const int BUF_LEN = 256;
    char buf_[BUF_LEN];
};

inline ScanFile::ScanFile() {
    verbose_ = false;
}

inline ScanFile::~ScanFile() {}

};

#endif


