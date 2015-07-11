#include "circuit.h"
#include "cell_mgr.h"
#include "library.h"
#include <iostream>
#include <string>
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
	string hashName = cellName;
	std::transform(hashName.begin(), hashName.end(), hashName.begin(), ::tolower);

	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i] == '\\')
			hashName.erase(hashName.begin()+i);
	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i+1] == '/')
			hashName[i] = '_';


	map<string, int>::iterator found = find_cell.find(hashName);
	if(found == find_cell.end())
		return -1;
	else
		return found->second;
}


int Circuit::getNetID(const string &netName)
{	
	string hashName = netName;
	std::transform(hashName.begin(), hashName.end(), hashName.begin(), ::tolower);

	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i] == '\\')
			hashName.erase(hashName.begin()+i);
	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i+1] == '/')
			hashName[i] = '_';

	map<string, int>::iterator found = find_net.find(hashName);
	if(found == find_net.end())
		return -1;
	else
		return found->second;
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

	for(unsigned i = 0 ; i < nets.size() ; ++i)
	{
		cout << "nets: " << nets[i].name << endl;
		cout << "  rise cap: " << nets[i].totalRiseCap << endl;
		cout << "  fall cap: " << nets[i].totalFallCap << endl;
	}
	
	return;
}

