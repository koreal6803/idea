// ************************************************************************** 
// File       [ circuit_simulator.h ]
// Author     [ koreal ]
// Synopsis   [ timed simulation ]
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
class Cell;
class Library;
class CircuitSimulator
{

public:
	
	/* construction */

	CircuitSimulator();
	CircuitSimulator(const CircuitSimulator &sc);
	~CircuitSimulator();

	/* initial circuit simulator */

	bool set(Circuit *circuit);
	bool set(Library *lib);
	bool set(const std::string &clkName , Wave *clkWave , double clkTime);
	bool set(std::vector<int> *piOrder, std::vector<int> *poOrder, std::vector<int> *ppiOrder);

	/* simulation */
	
	// propagate the initial values of all circuits from pi and ppi
	bool initial(std::string &pi , std::string &ppi);

	// set initial value of the net
	bool initial(int netID , bool value);

	// after initial value, we can get ppo1 (ppi2)
	std::string getPPO();

	// timed simulation to simulate wavefroms of nets
	bool simulate(std::string &pi , double piChangeTime , bool capture);	
	
	/* after timed simulation */
	
	// compare po and ppo value to the pattern set
	// true: means same
	// false: means different
	bool checkValue(std::string &po, std::string &ppo, bool capture);

	// get the waveform of nets
	Wave *getWave(){return netWave;}

private:
	
	void justifyInitialValue(Cell *cell);

	Circuit *cir;
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

#endif
