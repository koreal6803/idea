#include "pattern_set.h"

#include <sstream>
#include <iostream>
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
	cir = NULL;
	return true;

}

void PatternSet::set(Circuit *cir){
	this->cir = cir;
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
