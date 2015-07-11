#include "pattern_set.h"

#include <sstream>
#include <iostream>
#include <cassert>
#include "circuit.h"
using namespace std;
//#define USEDEBUG
#ifdef USEDEBUG
#define DBG(x) std::cout << x
#else
#define DBG(x) 
#endif

PatternSet::PatternSet()
{
	cir = NULL;
}

PatternSet::~PatternSet()
{
	fin.close();
}

bool PatternSet::setFile(const char * const fname)
{
	if(cir == NULL)
	{
		cout << "**ERROR: patternSet::read please set the circuit frist" <<endl;
		return false;
	}
	fin.open(fname);
	string line;
	while(getline(fin , line))
	{
		if(line[0] == '#')
			continue;
		stringstream ss(line);
		string command;
		ss >> command;
		if(command == "__PI_ORDER__")
		{
			string pi;
			while(ss >> pi)
				piOrder.push_back(cir->getNetID(pi));
		}
		else if(command == "__PO_ORDER__")
		{
			string po;
			while(ss >> po)
				poOrder.push_back(cir->getNetID(po));
		}
		else if(command == "__SCAN_ORDER__")
		{
			string sc;
			while(ss >> sc)
				ppiOrder.push_back(cir->getCellID(sc));
			break;
		}
	}
	// inverseFlipFlop = checkInverseFlipFlop();
	cir = NULL;
	return true;

}

void PatternSet::set(Circuit *cir){
	this->cir = cir;
}

vector<int> PatternSet::checkInverseFlipFlop()
{
	//cout << "check inverse ff" << endl;
	//cout << "circuit: " << cir->getName() << endl;
	vector<int> ret(ppiOrder.size() , -1);
	const vector<Cell> &cells = cir->getCells();
	const vector<Net> &nets = cir->getNets();
	for(unsigned i = 0 ; i < ppiOrder.size() ; ++i)
	{
		int ffID = ppiOrder[i];
		//cout << "ffID" << ffID << endl;
		const Cell &cell = cells[ffID];
		//cout << "cellName " << cell.name << endl;
		int siNetID = cell.ipt_net_id[2];
		//cout << "siID" << siNetID << endl;
		assert(siNetID != -1);
		const Net &si = nets[siNetID];
		int preCellID = si.ipt_cell_id;
		//cout << "preCellID" << preCellID << endl;
		if(preCellID == -1)
		{
			// scan in
			ret[i] = 0;
		}
		else
		{
			const Cell &preCell = cells[preCellID];
			if(siNetID == preCell.opt_net_id[0])//Q
				ret[i] = ret[i-1];
			else if(siNetID == preCell.opt_net_id[1])//QN
				ret[i] = (ret[i-1] == 1)?0:1;
		}

	}
	return ret;
}

bool PatternSet::readNextPattern()
{

	string line;
	stringstream ss;
	while(getline(fin , line))
	{
		if(line[0] == '#')
			continue;
		DBG( "pat line: " << line <<endl);
		string command;
		ss << line;
		ss >> command;
		if(command == "__PATTERN__")
			break;
		else
		{
			cout << "PatternSet::nextPat **ERROR there are non-pat line in pat file" <<endl;
			cout << line <<endl;
			return false;
		}
	}

	if(line =="")
		return false;
	
	pat.pis.clear();
	pat.pos.clear();
	pat.cycle.clear();

	int patternID;
	int captureCycle;
	ss >> patternID >> captureCycle;
	for(int i = 0 ; i < captureCycle ; i++)
	{
		string pi;
		string po;
		string cap;
		ss >> pi >> po >> cap;

		pat.pis.push_back(pi);
		pat.pos.push_back(po);
		if(cap == "__CAPT__")
			pat.cycle.push_back(Pattern::CAPTURE);
		else
			pat.cycle.push_back(Pattern::HOLD);

	}
	ss >> pat.ppi >> pat.ppo;
	/*
	for(unsigned i= 0 ; i < pat.ppo.size() ; ++i)
		if(pat.ppo[i] == '1')
			pat.ppo[i] = '0';
		else
			pat.ppo[i] = '1';
	//*/
	return true;
}

const Pattern PatternSet::getPattern()
{
	return pat;
}

bool PatternSet::empty()
{
	return fin.peek() == EOF;
}

void PatternSet::readAll()
{
	while(readNextPattern())
	{
		patterns.push_back(getPattern());
	}
}
