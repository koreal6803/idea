#include "circuit.h"
#include "cell_mgr.h"
#include <iostream>
using namespace std;


const string &Circuit::getName()
{
	return name;
}

const std::vector<Cell> &Circuit::getCells()
{
	return cells;
}
const std::vector<Net> &Circuit::getNets()
{
	return nets;
}

int Circuit::getCellID(const string &cellName)
{
	assert(find_cell.find(cellName) != find_cell.end());
	return find_cell[cellName];
}


int Circuit::getNetID(const string &netName)
{
	assert(find_net.find(netName) != find_net.end());
	return find_net[netName];
}

void Circuit::print()
{
	for(unsigned i = 0 ; i < cells.size() ; i++)
	{
		cout << "cell name: " << cells[i].name <<" level: " << cells[i].level <<endl;
		for(unsigned j = 0 ;j < cells[i].ipt_net_id.size() ; j++)
			if(cells[i].ipt_net_id[j] != -1)
				cout <<" ipt : " <<nets[cells[i].ipt_net_id[j]].name <<endl;
		for(unsigned j = 0 ;j < cells[i].opt_net_id.size() ; j++)
			if(cells[i].opt_net_id[j] != -1)
			{
				cout << " opt :"<<nets[cells[i].opt_net_id[j]].name <<endl;
			
			}
	}

	for(unsigned i = 0 ; i < po.size() ; ++i)
		cout << "PO " << "netID: " << po[i]<< " name: " << getNets()[po[i]].name << endl;
	for(unsigned i = 0 ; i < ppi.size() ; ++i)
		cout << "PPI " << "netID: " << ppi[i]<< " name: " << getNets()[ppi[i]].name << endl;
	for(unsigned i = 0 ; i < ppo.size() ; ++i)
		cout << "PPO " << "netID: " << ppo[i]<< " name: " << getNets()[ppo[i]].name << endl;
	
	return;
}
