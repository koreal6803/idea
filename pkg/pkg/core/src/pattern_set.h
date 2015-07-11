// **************************************************************************
// File       [ pat_file.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/13 created ]
// **************************************************************************

#ifndef __INTF_PAT_FILE_H__
#define __INTF_PAT_FILE_H__

#include <fstream>
#include <vector>
#include <string>
class Circuit;
class Pattern
{
public:
	enum CYCLE {CAPTURE , HOLD};
	std::vector<std::string> pis;
	std::vector<std::string> pos;
	std::vector<CYCLE> cycle;
	std::string ppi;
	std::string ppo;
};
class PatternSet
{
public:
	PatternSet();
	~PatternSet();
	void set(Circuit *cir);
	bool setFile(const char * const fname);
	void readAll();
	bool readNextPattern();
	bool empty();
	const Pattern getPattern();
	std::vector<int> piOrder;
	std::vector<int> poOrder;
	std::vector<int> ppiOrder;
	std::vector<Pattern> patterns;
private:
	std::ifstream fin;
	Circuit *cir;
	Pattern pat;
};


#endif
