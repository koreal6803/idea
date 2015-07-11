#ifndef __INTF_CIRCUIT_H__
#define __INTF_CIRCUIT_H__

// **************************************************************************
// File       [ circuit.h ]
// Author     [ koreal ]
// Synopsis   [ circuit structure , cell structure , net structure]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "wave.h"

class Circuit;
class Cell;
class Net;

// **************************************************************************
// Class      [ Circuit ]
// Author     [ koreal ]
// Synopsis   [ circuit structure ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
class Circuit
{
public:

	// get circuit name ex: s27
    const std::string &getName();

	// get cell array
	const std::vector<Cell> &getCells();

	// get net array
	const std::vector<Net> &getNets();

	// convert cell name to cell ID
	int getCellID(const std::string &cellName);

	// convert net name to cell ID
	int getNetID(const std::string &netName);

	// print the information of circuit
	void print();
	
	// net id of pi, po, ppi, ppo. ppi == ppo
	std::vector<int>		    pi,po,ppi,ppo;

	// cell id of flip flops
	std::vector<int>			ff;
	
	// flip flops pin name
	std::string dff_q;
	std::string dff_qn;
	std::string dff_d;
	std::string dff_si;
	std::string dff_se;
	std::string dff_ck;

	// convert cell type ID to string 
	std::map<int, std::string>	cell_types;

	// convert cell name to ID
	std::map<std::string,int> 	find_cell;

	// convert net name to ID
	std::map<std::string,int> 	find_net;

	// circuit name
	std::string 				name;

	// vector of cells and nets
	std::vector<Cell> 			cells;
	std::vector<Net> 			nets;
};

// logic type is set in cell.type
// logic type is used by timed simulation
enum Logic_type{
	AND = 0,	OR,
	XOR,		NAND,
	NOR,		XNOR,
	BUF,		INV,
	LOGIC1,		LOGIC0,
	DFF
};

// **************************************************************************
// Class      [ Cell ]
// Author     [ koreal ]
// Synopsis   [ cell structure ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

class Cell
{
public:
	Cell():type(-1),level(-1){}
	
	// cell type is the cell type id of .lib file
	// ex: NAND2_X2, NOR3_X2
    int type;

	// logic type is used by timed simulation
	Logic_type logic_type;
	
	// cell name
	std::string name;

	// level started from clk
	int level;
	
	// fanout net ID 
    std::vector<int> opt_net_id;
	
	// fanin net ID 
	std::vector<int> ipt_net_id;

	// convert ouput netID to pinID of the cell
	int getOptPin(int netID) const
	{
		std::vector<int>::const_iterator it = find(opt_net_id.begin() , opt_net_id.end(), netID);
		if(it == opt_net_id.end())
			return -1;
		else
			return it - opt_net_id.begin();
	}

	// convert input netID to pinID of the cell
	int getIptPin(int netID) const
	{
		std::vector<int>::const_iterator it = find(ipt_net_id.begin() , ipt_net_id.end(), netID);
		if(it == ipt_net_id.end())
			return -1;
		else
			return it - ipt_net_id.begin();
	}

};



// **************************************************************************
// Class      [ Net ]
// Author     [ koreal ]
// Synopsis   [ net structure ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

class Net
{
public:
	Net():ipt_cell_id(-1),totalRiseCap(0),totalFallCap(0){}

	// net name
    std::string name;

	// fanout cells
	std::vector<int> opt_cell_id;

	// driving cell
	int ipt_cell_id;

	// total capcitance
	double totalRiseCap;
	double totalFallCap;
};

#endif

