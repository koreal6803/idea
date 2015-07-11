
#ifndef __INTF_VLOG_FILE2_H__
#define __INTF_VLOG_FILE2_H__



class VlogFile {
public:
                 VlogFile();
    virtual      ~VlogFile();

    bool read(const char * const fname, const bool &verbose = false);

    virtual bool addModule(const char * const name);
    virtual bool addPorts(VlogNameList * const ports);
    virtual bool setInputNets(VlogNameList * const nets);
    virtual bool setOutputNets(VlogNameList * const nets);
    virtual bool setInoutNets(VlogNameList * const nets);
    virtual bool setWireNets(VlogNameList * const nets);
    virtual bool setRegNets(VlogNameList * const nets);
    virtual bool setSupplyLNets(VlogNameList * const nets);
    virtual bool setSupplyHNets(VlogNameList * const nets);

    virtual bool addInst(const char * const type, const char * const name,
                         VlogNameList * const ports);
    virtual bool addInst(const char * const type, const char * const name,
                         VlogPairList * const pairs);
    virtual bool addInst(const char * const type,
                         VlogNameList * const strengths,
                         const char * const name,
                         VlogNameList * const ports);
    virtual bool addAssign(const char * const n1, const char * const n2);

};

#endif
