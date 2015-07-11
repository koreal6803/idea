#ifndef __SP_MAKER__
#define __SP_MAKER__

#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class Measurement
{
public:
	double startTime;
	double gateDelay;
	double endTime;
};
class SpMaker
{
public:
	SpMaker(const std::string &fName);
	void add(const std::string &line);
	void addAlter(const std::string &line);
	void setMeasure(const std::string &node1 ,double value1, bool rise1, 
		 const std::string &node2, double value2, bool rise2 , const std::string& measureName);
	
	// hspice
	bool spiceSim();
	// nano sim
	bool nanoSim();

	// get measure
	Measurement getMeasure(const std::string &measureName);
	std::vector<Measurement> getAlterMeasure(const std::string &measureName);

private:
	
	// sim type
	enum SimulationType
	{
		SPICE,
		NANO_SIM,
		NOT_YET
	};

	SimulationType simType;

	// hspice get measure
	Measurement getMeasureSpice(const std::string &measureName);
	std::vector<Measurement> getAlterMeasureSpice(const std::string &measureName);
	
	// nano sim get measure
	Measurement getMeasureNano(const std::string &measureName);
	std::vector<Measurement> getAlterMeasureNano(const std::string &measureName);

	std::ofstream fout;
	std::string fName;
	int vsAmount;
};

#endif
