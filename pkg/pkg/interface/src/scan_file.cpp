// **************************************************************************
// File       [ scan_file.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/12/22 created ]
// **************************************************************************

#include <cstdio>

#include "scan_file.h"

using namespace std;
using namespace IntfNs;

bool ScanFile::read(const char * const fname, const bool &verbose) {

    verbose_ = verbose;

    FILE *scan_filein = fopen(fname, "r");
    if (!scan_filein) {
        fprintf(stderr, "**ERROR ScanFile::read(): cannot open scan file");
        fprintf(stderr, "`%s'\n", fname);
        return false;
    }

    // skip comments
    while (fgets(buf_, BUF_LEN, scan_filein) && buf_[0] == '#')
        ;

    // set number of chains
    int nchains;
    sscanf(buf_, "%*s %d", &nchains);
    if (nchains < 1) {
        if (verbose_)
            fprintf(stderr, "**ERROR ScanFile::read(): format error\n");
        return false;
    }
    if (!addGroup((size_t)nchains)) {
        if (verbose_)
            fprintf(stderr, "**ERROR ScanFile::read(): format error\n");
        return false;
    }

    char chain[BUF_LEN];
    char clk[BUF_LEN];
    char si[BUF_LEN];
    char so[BUF_LEN];
    char se[BUF_LEN];
    int len;
    for (int i = 0; i < nchains; ++i) {
        if (fscanf(scan_filein , "%s %s %s %s %s %d"
                , chain, clk, si, so, se, &len) != 6 || len < 1) {
            if (verbose_)
                fprintf(stderr, "**ERROR ScanFile::read(): format error\n");
            return false;
        }
        if (!addChain(chain, clk, si, so, se, len)) {
            if (verbose_) {
                fprintf(stderr, "**ERROR ScanFile::read(): format error\n");
            }
            return false;
        }
        char cell[BUF_LEN];
        char in[BUF_LEN];
        char out[BUF_LEN];
        char inv[5];
        for (int j = 0; j < len; ++j) {
            if (fscanf(scan_filein, "%s %s %s %s", cell, in, out, inv) != 4) {
                if (verbose_) {
                    fprintf(stderr, "**ERROR ScanFile::read(): ");
                    fprintf(stderr, "format error\n");
                }
                return false;
            }
            if (!addCell(cell, in, out, inv)) {
                if (verbose_) {
                    fprintf(stderr, "**ERROR ScanFile::read(): ");
                    fprintf(stderr, "format error\n");
                }
                return false;
            }
        }
    }

    fclose(scan_filein);

    return true;
}


bool ScanFile::addGroup(const size_t &nchains) {
    if (!verbose_)
        return true;
    printf("set group: %zu\n", nchains);
    return true;
}

bool ScanFile::addChain(const char * const name
    , const char * const clk
    , const char * const si
    , const char * const so
    , const char * const se
    , const int &len)
{
    if (!verbose_)
        return true;
    printf("add chain: %s %s %s %s %s %d\n", name, clk, si, so, se, len);
    return true;
}

bool ScanFile::addCell(const char * const name, const char * const in,
                       const char * const out, const char * const inv) {
    if (!verbose_)
        return true;
    printf("add cell: %s %s %s %s\n", name, in, out, inv);
    return true;
}


