#include "circuit_builder.h"
#include <cstdio>
#include <cassert>
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

#include "scan_chain_info.h"
#include "circuit.h"
#include "library.h"
using namespace IntfNs;
//using namespace IntfNs;

CircuitBuilder::CircuitBuilder(){
	cir = new Circuit;
	lib = NULL;
}

CircuitBuilder::~CircuitBuilder(){
	delete cir;
}

void CircuitBuilder::set(Library *lib)
{
	this->lib = lib;
}


bool CircuitBuilder::set(const std::string &q,
						   const std::string &qn,
						   const std::string &d,
						   const std::string &si,
						   const std::string &se,
						   const std::string &ck)
{
	cir->dff_q = q;
	cir->dff_qn = qn;
	cir->dff_d = d;
	cir->dff_si = si;
	cir->dff_se = se;
	cir->dff_ck = ck;
	return true;
}
bool levelCompare(const Cell &c1 , const Cell &c2)
{
	return c1.level < c2.level;
}

bool CircuitBuilder::addModule(const char * const name)
{
	cir->name = name;
	return true;
}

bool CircuitBuilder::addPorts(IntfNs::VlogNameList * const ports)
{
	return true;
}


bool CircuitBuilder::read(const char * const fname)
{
    extern int vlog_fileparse(void *);
    extern FILE *vlog_filein;
	
	if( lib == NULL)
	{
		cout << "Please set the lib and clkName to circuit Builder!" <<endl;
		return false;
	}

    verbose_ = false;
    vlog_filein = fopen(fname, "r");
    if (!vlog_filein) {
        fprintf(stderr, "**ERROR VlogFile::read(): cannot open netlist file");
        fprintf(stderr, "`%s'\n", fname);
        return false;
    }

    int res = vlog_fileparse(this);
    if (res != 0) {
        fprintf(stderr, "**ERROR VlogFile::read(): wrong input format\n");
        fclose(vlog_filein);
        return false;
    }
    fclose(vlog_filein);
	levelize();
	findCap();
	return true;
}

void CircuitBuilder::levelize()
{
	vector<Cell> &cells = cir->cells;
	vector<int> &pi = cir->pi;
	vector<int> &ff = cir->ff;
	// levelize all the cell
	
	// build nets fanout cells
	vector<vector<int> > netToCell(cir->nets.size() , vector<int>());
	// for each
	// cell
	for(unsigned c = 0 ; c < cells.size() ; c++)
	{
		// for each cell's ppi
		for(unsigned ipt = 0 ; ipt < cells[c].ipt_net_id.size() ; ipt++)
		{
			if(cells[c].ipt_net_id[ipt] == -1)
				continue;
			netToCell[cells[c].ipt_net_id[ipt]].push_back((int)c);
		}
	}
	// find clk pin
	
	int clkNetID = cir->find_net[clkName];
	

	// setting the level of clk tree
	queue<int> q;
	
	// put all the clk fanout cells to queue
	vector<int> &clk_fanout_cell = netToCell[clkNetID];
	for(unsigned i = 0 ; i < clk_fanout_cell.size() ; i++){
		int cell = clk_fanout_cell[i];
		cells[cell].level = 0;
		q.push(cell);
	}
	
	// visit the clk tree cells
	while(!q.empty())
	{
		int cell = q.front();
		q.pop();
		// find fanout cells : "opt_cell"
		for(unsigned i = 0 ; i < cells[cell].opt_net_id.size() ; i++){
			int net = cells[cell].opt_net_id[i];
			if(net == -1)
				continue;
			for(unsigned j = 0 ; j < netToCell[net].size() ; j++){
				int opt_cell = netToCell[net][j];
				if(cells[opt_cell].logic_type == DFF)
					continue;
				//set level
				cells[opt_cell].level = cells[cell].level+1;
				//push into queue
				q.push(opt_cell);
			}
		}
	}

	// setting the level of flip-flop
	int clk_tree_max_level = -1;
	for(unsigned i = 0 ; i < cells.size() ; i++){
		clk_tree_max_level = max(clk_tree_max_level , cells[i].level);
	}
	cout << "circuit cells      : " << cells.size() <<endl;
	cout << "circuit nets       : " << cir->nets.size() <<endl;
	cout << "clk tree max level : " << clk_tree_max_level <<endl;
	int ff_level = clk_tree_max_level +1;
	for(unsigned i = 0 ; i < cells.size() ; i++)
		if(cells[i].logic_type == DFF)
			cells[i].level = ff_level;
	cout << "dff level:         : " << ff_level <<endl;
	cout << "flip flop size     : " << ff.size() <<endl;

	// setting the level of other cells
	
	vector<bool> fix_level(cells.size());
	for(unsigned i = 0 ; i < cells.size() ; i++)
		fix_level[i] = (cells[i].level != -1);
	int pi_level = ff_level + 1;
	
	vector<int> visitCount(cells.size() , 0);
	for(unsigned i = 0 ;i < pi.size() ; i++){
		int net = pi[i];
		if(net == clkNetID)
			continue;
		for(unsigned j = 0; j < netToCell[net].size() ; j++){
			int fanout_cell = netToCell[net][j];
			if(fix_level[fanout_cell])
				continue;
			cells[fanout_cell].level = pi_level;
			q.push(fanout_cell);
		}
	}
	for(unsigned i = 0 ; i < ff.size() ; i++)
		q.push(ff[i]);
	

	// visit all the cell by topological order
	while(!q.empty())
	{
		int cell = q.front();
		q.pop();
		// find fanout cells : "opt_cell"
		for(unsigned i = 0 ; i < cells[cell].opt_net_id.size() ; i++){
			int net = cells[cell].opt_net_id[i];
			if(net == -1)
				continue;
			for(unsigned j = 0 ; j < netToCell[net].size() ; j++){
				int opt_cell = netToCell[net][j];
				if(fix_level[opt_cell])
					continue;
				if(cells[cell].level+1 > cells[opt_cell].level){
					//set level
					cells[opt_cell].level = cells[cell].level+1;
					//push into queue
					q.push(opt_cell);
				}
			}
		}
	}
	
	

	for(unsigned i = 0 ; i < cells.size() ; i++){
	//	cout << cells[i].name << " level: " << cells[i].level <<endl;
		if(cells[i].level == -1){
			cout << "levelization fail! " << cells[i].name <<" has no level"<<endl;
			assert(0);
		}
	}
	
	sort(cells.begin() , cells.end() , levelCompare);
	for(unsigned i = 0 ; i < cells.size() ; i++)
		cir->find_cell[cells[i].name] = i;
	ff.clear();
	for(unsigned i = 0 ; i < cells.size() ; i++)
		if(cells[i].logic_type == DFF)
			ff.push_back(i);
	
	if(cir->dff_d != "")
		for(unsigned i = 0 ; i < ff.size() ; ++i)
		{
			int cellID = ff[i];
			int pinD = lib->getFanInIndex(cir->cells[cellID].type, cir->dff_d);
			int pinQ = lib->getFanOutIndex(cir->cells[cellID].type, cir->dff_q);
			cir->ppi.push_back(cir->cells[cellID].opt_net_id[pinQ]);
			cir->ppo.push_back(cir->cells[cellID].ipt_net_id[pinD]);
			
		}
	
	for(unsigned i = 0 ; i < cells.size() ; ++i)
	{
		Cell &cell = cir->cells[i];
		for(unsigned j = 0 ; j < cell.opt_net_id.size() ; ++j)
		{
			int netID = cell.opt_net_id[j];
			if(netID != -1)
				cir->nets[netID].ipt_cell_id = i;
		}
		for(unsigned j = 0 ; j < cell.ipt_net_id.size() ; ++j)
		{
			int netID = cell.ipt_net_id[j];
			if(netID != -1)
				cir->nets[netID].opt_cell_id.push_back(i);
		}
	}
    return;
}

void CircuitBuilder::findCap()
{
	vector<Net> &nets = cir->nets;
	LookUpTableInfo info;
	for(unsigned netID = 0 ; netID < nets.size() ; ++netID)
	{
		Net &net = nets[netID];
		net.totalRiseCap = 0;
		net.totalFallCap = 0;
		for(unsigned j = 0  ; j < net.opt_cell_id.size() ; ++j)
		{
			Cell &optCell = cir->cells[net.opt_cell_id[j]];
			vector<int>::iterator it = find(optCell.ipt_net_id.begin() , optCell.ipt_net_id.end() , netID);
			assert(it != optCell.ipt_net_id.end());
			int pinID = it - optCell.ipt_net_id.begin();
			info.cellIndex = optCell.type;
			info.relativePinIndex = pinID;
			info.toggleRise = true;
			net.totalRiseCap += lib->getCapacitance(info);
			info.toggleRise = false;
			net.totalFallCap += lib->getCapacitance(info);
		}
	}
}
bool CircuitBuilder::setInputNets(IntfNs::VlogNameList * const nets)
{
	for(VlogNameList::iterator it = nets->begin() ; it != nets->end() ; it++)
	{
	    Net new_net;
		new_net.name = *it;
		cir->pi.push_back(cir->nets.size());
		cir->find_net[new_net.name] = cir->nets.size();
		cir->nets.push_back(new_net);
	}
	return true;
}
bool CircuitBuilder::setOutputNets(IntfNs::VlogNameList * const nets)
{
	for(VlogNameList::iterator it = nets->begin() ; it != nets->end() ; it++)
	{
	    Net new_net;
		new_net.name = *it;
		cir->po.push_back(cir->nets.size());
		cir->find_net[new_net.name] = cir->nets.size();
		cir->nets.push_back(new_net);
	}
	return true;
}
bool CircuitBuilder::setWireNets(IntfNs::VlogNameList * const nets)
{
	for(VlogNameList::iterator it = nets->begin() ; it != nets->end() ; it++)
	{
	    Net new_net;
		new_net.name = *it;
		cir->find_net[new_net.name] = cir->nets.size();
		cir->nets.push_back(new_net);
	}
	return true;
}
bool CircuitBuilder::setSupplyLNets(IntfNs::VlogNameList * const nets)
{
	// no supply net will be parse
	// because the virilog file should be routed!
	return false;
}
bool CircuitBuilder::setSupplyHNets(IntfNs::VlogNameList * const nets)
{
	// no supply net will be parse
	// because the virilog file should be routed!
	return false;
}

bool CircuitBuilder::addInst(const char * const type, const char * const name,
                      IntfNs::VlogPairList * const pairs)
{
	string cell_type(type);
	

	Cell new_cell;
	new_cell.name = string(name);
	new_cell.type = lib->getCellIndex(cell_type);
	assert(new_cell.type != -1);
	if(cell_type.find("NAND")!=string::npos)
	    new_cell.logic_type = NAND;
	else if(cell_type.find("AND")!=string::npos)
		new_cell.logic_type = AND;
	else if(cell_type.find("XNOR")!=string::npos)
	    new_cell.logic_type = XNOR;
	else if(cell_type.find("XOR")!=string::npos)
	    new_cell.logic_type = XOR;
	else if(cell_type.find("NOR")!=string::npos)
	    new_cell.logic_type = NOR;
	else if(cell_type.find("OR")!=string::npos)
	    new_cell.logic_type = OR;
	else if(cell_type.find("INV")!=string::npos)
	    new_cell.logic_type = INV;
	else if(cell_type.find("BUF")!=string::npos)
	    new_cell.logic_type = BUF;
	else if(cell_type.find("LOGIC1")!=string::npos)
		new_cell.logic_type = LOGIC1;
	else if(cell_type.find("LOGIC0")!=string::npos)
		new_cell.logic_type = LOGIC0;
	else if(cell_type.find("DFF")!=string::npos)
	    new_cell.logic_type = DFF;
	else
	{
		cout << "there is no such complex logic: " << cell_type <<endl;
		return false;
	}

	if(new_cell.logic_type == DFF)
		cir->ff.push_back(cir->cells.size());
	
	if(new_cell.logic_type == LOGIC1 || new_cell.logic_type == LOGIC0){
		new_cell.level = 0;
	}
	new_cell.ipt_net_id.assign(lib->getFanInSize(new_cell.type),-1);
	new_cell.opt_net_id.assign(lib->getFanOutSize(new_cell.type),-1);
	
	for(VlogPairList::iterator it = pairs->begin() ; it != pairs->end() ; it++)
	{
		 string pinName = it->first;
		 string netName = it->second;

		 assert(cir->find_net.find(netName)!=cir->find_net.end());
		 int netID = cir->find_net[netName];
		 bool output = lib->isFanOut(new_cell.type , pinName);
		 if(output)
		 {
			 int pinID = lib->getFanOutIndex(new_cell.type , pinName);
			 assert(pinID != -1); 
		 	 new_cell.opt_net_id[pinID] = netID;
		 }
		 else
		 {
			 int pinID = lib->getFanInIndex(new_cell.type , pinName);
			 assert(pinID != -1);
			 new_cell.ipt_net_id[pinID] = netID;
		 }
	}
	cir->cells.push_back(new_cell);

	return true;
}
bool CircuitBuilder::addAssign(const char * const n1, const char * const n2)
{
	
	int net1 = cir->find_net[n1];
	int net2 = cir->find_net[n2];
	
	if(net2 > net1)
		net2--;
	
	for(map<string,int>::iterator it = cir->find_net.begin() ; it != cir->find_net.end() ; it++)
	{
		if(it->second == net1)
			it->second = net2;
		else if(it->second > net1)
			it->second--;
	}
	for(unsigned i = 0 ; i < cir->po.size()  ;++i)
		if(cir->po[i] == net1)
			cir->po[i] = net2;

	vector<Cell> &cells = cir->cells;
	cir->nets.erase(cir->nets.begin() + net1);
	for(unsigned i = 0 ; i < cir->cells.size() ; i++)
	{
		for(unsigned j = 0 ; j < cells[i].opt_net_id.size() ; j++)
		{
			if(cells[i].opt_net_id[j] == -1)
				continue;
			if(cells[i].opt_net_id[j] > net1)
				cells[i].opt_net_id[j]--;
			else if(cells[i].opt_net_id[j] == net1)
				cells[i].opt_net_id[j] = net2;
		}
		for(unsigned j = 0 ; j < cells[i].ipt_net_id.size() ; j++)
		{
			if(cells[i].ipt_net_id[j] == -1)
				continue;
			if(cells[i].ipt_net_id[j] > net1)
				cells[i].ipt_net_id[j]--;
			else if(cells[i].ipt_net_id[j] == net1)
				cells[i].ipt_net_id[j] = net2;
		}
	}
	return true;
}

void CircuitBuilder::set(std::string clkName)
{
	this->clkName = clkName;
}

Circuit CircuitBuilder::getCircuit()
{
	return *cir;
}


