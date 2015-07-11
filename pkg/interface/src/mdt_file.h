// **************************************************************************
// File       [ mdt_file.h ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/06/23 created ]
// **************************************************************************

#ifndef _INTF_MDT_FILE_H_
#define _INTF_MDT_FILE_H_

#include <list>

namespace IntfNs {

enum MdtPortType { MDT_PORT_INPUT
    , MDT_PORT_OUTPUT
    , MDT_PORT_INOUT
    , MDT_PORT_INTERN
};

typedef std::list<char *>                     MdtNameList;
typedef std::list<std::pair<char *, char *> > MdtPairList;

class MdtFile {
public:
                 MdtFile();
    virtual      ~MdtFile();

    virtual bool read(const char * const fname, const bool &verbose = false);

    virtual bool addModel(const char * const name);
    virtual bool addPorts(MdtNameList * const ports);
    virtual bool setInputNets(MdtNameList * const nets);
    virtual bool setOutputNets(MdtNameList * const nets);
    virtual bool setInoutNets(MdtNameList * const nets);
    virtual bool setInternNets(MdtNameList * const nets);
    virtual bool addPrimitive(const char * const type
        , const char * const name
        , MdtNameList * const ports);
    virtual bool addInstance(const char * const type
        , const char * const name
        , MdtNameList * const ports);
    virtual bool addInstance(const char * const type
        , const char * const name
        , MdtPairList * const pairs);

protected:
    bool verbose_;
};

inline MdtFile::MdtFile() {
    verbose_ = false;
}

inline MdtFile::~MdtFile() {}

};

#endif

