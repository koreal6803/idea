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
#include "circuit.h"
#include "interface/src/vlog_file.h"

//class in this header file:
class Circuit;
class Library;

//Circuit
class CircuitBuilder
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
	Circuit  cir;
	Library  *lib;
	std::string clkName;
	void levelize();
	void findCap();

    bool addModule(std::string &name);
    bool addPorts(std::string &port);
    bool setInputNets( std::string net);
    bool setOutputNets(std::string &net);
    bool setWireNets(std::string &net);
    bool setSupplyLNets(std::string &nets);
    bool setSupplyHNets(std::string &nets);

    bool addInst(std::string  &type, std::string &name,
                 std::vector<std::string> &ports , std::vector<std::string> &nets);
    bool addAssign(std::string &n1 , std::string &n2);
};


#endif
