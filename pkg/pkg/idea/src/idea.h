// **************************************************************************
// File       [ circuit.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
#ifndef __INTF_IDEA_H__
#define __INTF_IDEA_H__
#include <vector>
#include "ir_core/src/power_grid_solver.h"
class Wave;
class Circuit;
class DelayInfo;
class Transition;

class Idea
{
public:
	Idea();
	void set(pgNs::PowerGridSolver *pgs);
	void set(Circuit *cir);
	void set(double clkCycle);
	void set(DelayInfo *delayInfo);
	void run(Wave *wave);
	bool checkViolate();
	
	std::vector<std::vector<double> > current;
	std::vector<std::vector<double> > voltage;
	double maxPathDelay;
	double maxTransitionPeriod;
	std::vector<std::vector<Transition*> > stepTransitions;
private:
	Circuit *cir_;
	double clkCycle_;
	DelayInfo *delayInfo;
	pgNs::PowerGridSolver *pgs_;
	std::vector<int> cellVDDNodeID;
	std::vector<int> cellGNDNodeID;


};

#endif
