// ************************************************************************** // File       [ circuit_simulator.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
#ifndef __CORE_CIRCUIT_SIMULATOR_H__
#define __CORE_CIRCUIT_SIMULATOR_H__

#include <utility>
#include <vector>
#include <string>
#include "wave.h"
class Wave;
class Circuit;
class Pattern;
class DelayInfo;
class Cell;
class Library;
class CircuitSimulator
{

public:

	CircuitSimulator();
	CircuitSimulator(const CircuitSimulator &sc);
	~CircuitSimulator();
	// settings before simulation
	bool set(Circuit *circuit);
	bool set(DelayInfo *delayInfo);
	bool set(Library *lib);
	bool set(const std::string &clkName , Wave *clkWave , double clkTime);
	bool set(std::vector<int> *piOrder, std::vector<int> *poOrder, std::vector<int> *ppiOrder);

	// simulation
	bool initial(std::string &pi , std::string &ppi);    //optional
	bool initial(int netID , bool value);				//optional
	std::string getPPO();
	bool simulate(std::string &pi , double piChangeTime , bool capture);	
	bool untimeSimulate(std::string &pi , double piChangeTime , bool capture);	
	
	bool checkValue(std::string &po, std::string &ppo, bool capture);		//optional
	bool checkTiming();										//optional
	Wave *getWave(){return netWave;}

private:
	
	void justifyInitialValue(Cell *cell);

	Circuit *cir;
	DelayInfo *delayInfo;
	Library *lib;
	double cycleTime;
	int clkID;
	Wave clkWave;


	Wave temp;
	std::vector<int> piOrder;
	std::vector<int> ppiOrder;
	std::vector<int> poOrder;

	Wave *netWave;
};


typedef std::vector<Wave>::iterator WaveIter;


#endif
