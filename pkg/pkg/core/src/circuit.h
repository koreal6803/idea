
// **************************************************************************
// File       [ circuit.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
#ifndef __INTF_CIRCUIT_H__
#define __INTF_CIRCUIT_H__

#include <string>
#include <vector>
#include <map>
#include "wave.h"
class Circuit;
class Cell;
class Net;
class CellMgr;

class Circuit
{
friend class CircuitBuilder;
friend class CircuitSimulator;
public:
    const std::string &getName();
	const std::vector<Cell> &getCells();
	const std::vector<Net> &getNets();
	int getCellID(const std::string &cellName);
	int getNetID(const std::string &netName);
	void print();
	
	std::vector<int>		    pi,po,ppi,ppo;
	std::vector<int>			ff;

	std::string dff_q;
	std::string dff_qn;
	std::string dff_d;
	std::string dff_si;
	std::string dff_se;
	std::string dff_ck;
private:

	std::string 				name;
	std::vector<Cell> 			cells;
	std::vector<std::string>	cell_types;
	std::vector<Net> 			nets;
	std::map<std::string,int> 	find_cell;
	std::map<std::string,int> 	find_net;

};


enum Logic_type{
	AND = 0,
	OR,
	XOR,
	NAND,
	NOR,
	XNOR,
	BUF,
	INV,
	LOGIC1,
	LOGIC0,
	DFF
};

//Cell
class Cell
{
public:
	Cell():type(-1),level(-1){}

    int type;
	Logic_type logic_type;
	
	std::string name;
	int level;
	
    std::vector<int> opt_net_id;

	std::vector<int> ipt_net_id;
};



//Net
class Net
{
public:
	Net():ipt_cell_id(-1){}
    std::string name;
	std::vector<int> opt_cell_id;
	int ipt_cell_id;
	Wave wave;
	double totalRiseCap;
	double totalFallCap;
};

#endif

