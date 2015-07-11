#ifndef __INTF_IDEA_H__
#define __INTF_IDEA_H__
// **************************************************************************
// File       [ idea.h ]
// Author     [ koreal ]
// Synopsis   [ idea circuit simulator ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

/* library */
#include <vector>
#include <fstream>

/* library */
#include "matrix/src/matrix.h"
#include "ir_core/src/power_grid_solver.h"

/* class */
class Wave;
class Circuit;
class Transition;
class Library;

// **************************************************************************
// Class      [ Idea result ]
// Author     [ koreal ]
// Synopsis   [ results after idea simulation ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
class IdeaResult
{
public:
	
	// original path daley without IR-drop effect
	double originalPathDelay;

	// There are different path delay according to different vth
	std::vector<double> powerAwarePathDelay;

	// window size ex: 0.3ns
	double windowSize;

	// save the voltage for each vdd and gnd cell
	// usage: voltage[windowID][powerGridNodeID]
	std::vector<matrixNs::Vector> voltage;
	
	// initialize function
	IdeaResult(double originalPathDelay , std::vector<double> powerAwarePathDelay , double windowSize , std::vector<matrixNs::Vector > &voltage):
		originalPathDelay(originalPathDelay),
		powerAwarePathDelay(powerAwarePathDelay),
		windowSize(windowSize),
		voltage(voltage)
	{};
};

// **************************************************************************
// Class      [ IDEA ]
// Author     [ koreal ]
// Synopsis   [ idea simulation ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
class Idea
{
friend class PowerInfo;
public:
	// constructor
	Idea();

	// initial setting
	void set(pgNs::PowerGridSolver *pgs);
	void set(Circuit *cir);
	void set(double clkCycle);
	void set(Library *lib);
	
	// simulation
	IdeaResult run(Wave *wave , std::vector<double> &vth);
	
	// result variables
	//
	// average voltage of every transition
	std::vector<double> vls;
	std::vector<double> vhs;

	// different extra gate delay consider vth for every transition
	// ex: extraGateDelay[vthID][transitionID]
	std::vector<std::vector<double > >  extraGateDelays;

	// map transition location to transition ID
	std::map<const Transition*,int> transitionIDs;
	
	// get the power grid pointer of idea
	pgNs::PowerGridSolver *pgs_;

private:

	Circuit *cir_;
	std::vector<matrixNs::Vector> currentToVoltage;
	
	void getMaxDelay(Wave *wave, double &pathPathDelay , double &maxGateDelay);
	void getMaxDelay(Wave *wave , double &originalMaxPathDelay);
	void getWindows(double windowWidth , const Transition &t , int &window1 , int &window2 , double &portion1 , double &portion2);
	void getWindows(double windowWidth , const Transition &t , std::vector<int> &windows , std::vector<double> &portions);
	void getOutputSwing(int cellID , int window1 , int window2 , double portion, double &voh , double &vol);
	void getOutputSwing(int cellID , std::vector<int> &windows , std::vector<double> &portions, double &voh , double &vol);
	void getInputSwing(int preNetID , int window1 , int window2 , double portion , double &vih , double &vil);
	void getInputSwing(int preNetID , std::vector<int> &windows , std::vector<double> &portions , double &vih , double &vil);
	void getPsnAwarePathDelay(Wave *wave , std::vector<int> &nets , std::vector<double> &maxPsnAwarePathDelay);

	double clkCycle_;
	Library *lib_;
	std::vector<int> cellVDDNodeID;
	std::vector<int> cellGNDNodeID;

	std::vector<matrixNs::Vector> &calculateCurrent(Wave *wave , double windowWidth);
	
	
	double windowSize;
};

#endif
