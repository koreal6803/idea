// 
// **************************************************************************
// File       [ circuit.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#ifndef __INTF_CIRCUIT_BUILDER_H__
#define __INTF_CIRCUIT_BUILDER_H__

#include <vector>
#include <map>
#include <string>
#include "interface/src/vlog_file.h"

//class in this header file:
class Circuit;
class Library;

//Circuit
class CircuitBuilder : public IntfNs::VlogFile
{
public:
	CircuitBuilder();
	~CircuitBuilder();
	
	void set(Library *lib);
	void set(std::string clkName);
	bool set(const std::string &q , 
			 const std::string &qn , 
			 const std::string &d ,
			 const std::string &si ,
			 const std::string &se ,
			 const std::string &ck);

    bool read(const char * const fname);
	Circuit getCircuit();


private:
	Circuit  *cir;
	Library  *lib;
	std::string clkName;
	void levelize();
	void findCap();

    bool addModule(const char * const name);
    bool addPorts(IntfNs::VlogNameList * const ports);
    bool setInputNets(IntfNs::VlogNameList * const nets);
    bool setOutputNets(IntfNs::VlogNameList * const nets);
    bool setWireNets(IntfNs::VlogNameList * const nets);
    bool setSupplyLNets(IntfNs::VlogNameList * const nets);
    bool setSupplyHNets(IntfNs::VlogNameList * const nets);

    bool addInst(const char * const type, const char * const name,
                 IntfNs::VlogPairList * const pairs);
    bool addAssign(const char * const n1, const char * const n2);
};


#endif
