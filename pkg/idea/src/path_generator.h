#ifndef __UTILITY_PATH_GENERATOR
#define __UTILITY_PATH_GENERATOR

#include <vector>
#include <string>
#include <map>
#include "ir_core/src/power_grid.h"

class Circuit;
class Pattern;
class PowerGrid;
class SpMaker;

class PathGenerator
{
public:
	enum SimType
	{
		HSPICE,NANO_SIM
	};

	PathGenerator(Circuit *cir,double vdd):cir_(cir),vdd_(vdd){}
	bool set(std::vector<int> *piOrder , std::vector<int> *poOrder, std::vector<int> *ppiOrder);
	bool set(pgNs::PowerGrid *pg);
	bool set(std::string workspace);
	bool sim(Pattern *pat, double piTime, double piPeriod, SimType simType,int no);
	bool extractResult(SimType type , std::string workspace , int no);
	void save(const std::string &fileName);

	
	std::map<std::string, double> pathDelay_;
	std::map<std::string, double> extraPathDelay_;

private:
	void setMeasurement(SpMaker *sp);
	Circuit *cir_;
	pgNs::PowerGrid *pg_;
	double vdd_;
	int pinQ_;
	int pinQN_;
	std::vector<int> *piOrder;
	std::vector<int> *poOrder;
	std::vector<int> *ppiOrder;

	std::string workspace;

};

#endif
