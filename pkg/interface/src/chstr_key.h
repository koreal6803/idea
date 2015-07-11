// **************************************************************************
// File       [ chstr_key.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/09/29 created ]
// **************************************************************************

#ifndef __INTF_CHSTR_KEY_H__
#define __INTF_CHSTR_KEY_H__

#include <cstring>

namespace IntfNs {

struct ChstrKey {
         ChstrKey(const ChstrKey &key);
         ChstrKey(const char * const val);
         ~ChstrKey();
    bool operator<(const ChstrKey &key) const;
    char *val_;
};

inline ChstrKey::ChstrKey(const ChstrKey &key) {
    val_ = strdup(key.val_);
}

inline ChstrKey::ChstrKey(const char * const val) {
    val_ = strdup(val);
}

inline ChstrKey::~ChstrKey() {
    delete [] val_;
}

inline bool ChstrKey::operator<(const ChstrKey &key) const {
    return strcmp(val_, key.val_) < 0;
}

};

#endif

