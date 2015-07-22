
/* library */
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <fstream>

/* library */
#include "idea.h"
#include "core/src/wave.h"
#include "core/src/circuit.h"
#include "core/src/library.h"
#include "ir_core/src/power_grid.h"


/* cout only when debug flag is on */
#ifdef USEDEBUG
#define DBG(x) std:://cout << x
#define DBGF(x) x
#else
#define DBG(x) 
#define DBGF(x)
#endif



/* namespace */
using namespace std;
using namespace pgNs;

Idea::Idea()
{
	cir_ = NULL;
	pgs_ = NULL;
	lib_ = NULL;
}

void Idea::set(Circuit *cir)
{
	if(pgs_ == NULL)
	{
		cout << "please setup power grid solver first!"<<endl;
		return;
	}
	cir_ = cir;
	PowerGrid *pg = pgs_->getPowerGrid();
	const vector<Cell> cells = cir->getCells();
	for(unsigned i = 0 ; i < cells.size() ; ++i)
	{
		int vddNode = pg->getPowerNode(cells[i].name);
		int gndNode = pg->getGroundNode(cells[i].name);
		if(vddNode == -1)
		{
			cout << "**ERROR Idea::setCircuit: cannot find vddNode of cell " << cells[i].name << endl;
			exit(0);
		}
		if(gndNode == -1)
		{
			cout << "**ERROR Idea::setCircuit: cannot find gndNode of cell " << cells[i].name << endl;
			exit(0);
		}
		cellVDDNodeID.push_back(vddNode);
		cellGNDNodeID.push_back(gndNode);
	}
}

void Idea::set(PowerGridSolver *pgs)
{
	pgs_ = pgs;
}

void Idea::set(double clkCycle)
{
	clkCycle_ = clkCycle;
}

void Idea::set(Library *lib)
{
	lib_ = lib;
}

void Idea::getOutputSwing(int cellID , vector<int> &windows , vector<double> &portions, double &voh , double &vol)
{
	// find vdd node and gnd node id of power gird
	int vddNode = cellVDDNodeID[cellID];
	int gndNode = cellGNDNodeID[cellID];

	// reset voh and vol
	voh = 0;
	vol = 0;

	// find voltage high and voltage low in each window
	for(unsigned i = 0 ; i < windows.size() ; ++i)
	{
		int window = windows[i];
		double portion = portions[i];
		voh += currentToVoltage[window].get(vddNode) * portion;
		vol += currentToVoltage[window].get(gndNode) * portion;
	}
}

void Idea::getInputSwing(int prevNetID , vector<int> &windows , vector<double> &portions , double &vih , double &vil)
{
	assert(prevNetID != -1);

	// find previous cell
	int prevCellID = cir_->nets[prevNetID].ipt_cell_id;

	if(prevCellID == -1)
	{
		vih = pgs_->getPowerGrid()->getSupplyVoltage();
		vil = 0;
		return;
	}

	// reset vih and vih
	vih = 0;
	vil = 0;

	// find vdd node and gnd node id of power gird
	int vddNode = cellVDDNodeID[prevCellID];
	int gndNode = cellGNDNodeID[prevCellID];

	// find voltage high and voltage low in each window
	for(unsigned i = 0 ; i < windows.size() ; ++i)
	{
		int window = windows[i];
		double portion = portions[i];
		vih += currentToVoltage[window].get(vddNode) * portion;
		vil += currentToVoltage[window].get(gndNode) * portion;
	}
}

void Idea::getWindows(double windowWidth , const Transition &transition , vector<int> &windows , vector<double> &portions)
{
	double startTime = transition.prevTransition->time;
	double endTime   = transition.time;
	
	int startWindow = startTime / windowWidth;
	int endWindow = endTime / windowWidth;

	for(int w = startWindow ; w <= endWindow ; ++w)
	{
		windows.push_back(w);
		double portion;
		if(w == startWindow)
			portion = (double) ( (startWindow + 1) * windowWidth - startTime ) / (endTime - startTime);
		else if(w == endWindow)
			portion = (double) ( endTime - endWindow * windowWidth ) / (endTime - startTime);
		else
			portion = (double) windowWidth / (endTime - startTime);
		if(portion > 1)
			portion = 1;
		portions.push_back(portion);

	}
}

void Idea::getMaxDelay(Wave *wave , double &originalMaxPathDelay , double &maxGateDelay)
{
	// initial latest event and largest delay
	originalMaxPathDelay = 0.1; // 1ns
	maxGateDelay = 0.1;// 1ns

	// search all net to find latest event and largest gate delay
	for(unsigned netID = 0 ; netID < cir_->nets.size() ; ++netID)
	{
		// skip if net has no transition
		if(wave[netID].transition.size() == 0)
			continue;

		// get the latest transition of the wave
		const Transition *t = &wave[netID].transition.back();

		// calculate max path delay
		originalMaxPathDelay = max(t->time + t->period*2.5 , originalMaxPathDelay);

		// calculate max gate delay
		if(t->prevTransition)
			maxGateDelay = max(t->time - t->prevTransition->time , maxGateDelay);
	}
}

void Idea::getMaxDelay(Wave *wave , double &originalMaxPathDelay)
{
	// initial latest event and largest delay
	originalMaxPathDelay = 0.1; // 1ns

	// search all net to find latest event and largest gate delay
	for(unsigned i = 0 ; i < cir_->ppo.size() ; ++i)
	{
		int netID = cir_->ppo[i];
		// skip if net has no transition
		if(wave[netID].transition.size() == 0)
			continue;

		// get the latest transition of the wave
		const Transition *t = &wave[netID].transition.back();

		// calculate max path delay
		originalMaxPathDelay = max(t->time , originalMaxPathDelay);
	}
	// search all net to find latest event and largest gate delay
	for(unsigned i = 0 ; i < cir_->po.size() ; ++i)
	{
		int netID = cir_->po[i];
		// skip if net has no transition
		if(wave[netID].transition.size() == 0)
			continue;

		// get the latest transition of the wave
		const Transition *t = &wave[netID].transition.back();

		// calculate max path delay
		originalMaxPathDelay = max(t->time , originalMaxPathDelay);
	}
}

void Idea::getPsnAwarePathDelay(Wave *wave , vector<int> &nets , vector<double> &maxPsnAwarePathDelay)
{
	// calculate how many path delay we need to calculate
	unsigned vthSize = maxPsnAwarePathDelay.size();

	// for every po and ppo
	for(unsigned i = 0 ; i < nets.size() ; ++i)
	{
		int netID = nets[i];

		// if the wave contain no transition
		// then continue
		if(wave[netID].transition.empty())
			continue;

		// find the latest transition of the wave
		const Transition *t = &wave[netID].transition.back();
		vector<double> psnAwarePathDelay(vthSize , t->time);
		
		// accumulate the extra gate delay along the path
		while(t->prevTransition)
		{
			int transitionID = transitionIDs[t];
			for(unsigned i = 0 ; i < vthSize ; ++i)
				psnAwarePathDelay[i]+=extraGateDelays[i][transitionID];
			
			t = t->prevTransition;
		}

		// choose max PSN-aware path delay for every vth
		for(unsigned i = 0 ; i < vthSize ; ++i)
			if(psnAwarePathDelay[i] > maxPsnAwarePathDelay[i])
				maxPsnAwarePathDelay[i] = psnAwarePathDelay[i];
	}
}

IdeaResult Idea::run(Wave *wave , std::vector<double> &vths)
{
	// check before runing idea
	if(lib_ == NULL || cir_ == NULL || pgs_ == NULL)
	{
		cout << "please setting before run Idea" << endl;
		IdeaResult ideaResult(-1.0,vector<double>(),-1,currentToVoltage);
		return ideaResult;
	}
	
	/* initial the required informations */

	// calculate originalMaxPathDelay and maxGateDelay
	double maxPathDelay;
	double maxGateDelay;
	getMaxDelay(wave , maxPathDelay , maxGateDelay);
	
	// get supply voltage vdd
	double vdd = pgs_->getPowerGrid()->getSupplyVoltage();

	// set window width
	double windowWidth = maxGateDelay;

	// set time step
	int windowSize = int(maxPathDelay/windowWidth+1);
	
	/* calculate extra gate delay */

	// set current size
	currentToVoltage.clear();
	currentToVoltage.assign(windowSize , matrixNs::Vector(pgs_->matrixSize()));
	currentToVoltage = calculateCurrent(wave , windowWidth);

	/* calculate average PSN in every window */
	for(unsigned i = 0 ; i < currentToVoltage.size() ; ++i)
		pgs_->solve(&currentToVoltage[i]);
	
	/* calculate extra gate delay */

	// clear extra gate delay of previous pattern
	extraGateDelays.clear();
	vls.clear();
	vhs.clear();
	extraGateDelays.assign(vths.size(),vector<double>());
	transitionIDs.clear();

	// calculate extra gate delay
	const vector<Net>  & nets = cir_->getNets();
	const vector<Cell> & cells = cir_->getCells();
	for(unsigned i = 0 ; i < nets.size() ; ++i)
	{
		int cellID = nets[i].ipt_cell_id;

		// there are no cell to drive this net. ex: pi
		if(cellID == -1)
			continue;

		// calculate extra gate delay of each transitions in wave
		for(unsigned j = 0 ; j < wave[i].transition.size() ; ++j)
		{
			Transition &transition = wave[i].transition[j];
			assert(transition.iptPin != -1);

			// calculate window
			vector<int> windows;
			vector<double> portions;
			getWindows(windowWidth , transition , windows , portions);
			
			/* calculate vos */
			double vol;
			double voh;
			getOutputSwing(cellID , windows , portions , voh , vol);

			double vos = voh - vol;
			double deltaVos = vos - vdd;
			
			/* calculate vsw1 */
			double vil;
			double vih;
			int prevNetID = cells[cellID].ipt_net_id[transition.iptPin];
			getInputSwing(prevNetID , windows , portions , vih , vil);

			double vis = (transition.value == Wave::H)? voh - vil : vih - vol;
			double deltaVis = vis - vdd;

			double originalGateDelay = transition.time - transition.prevTransition->time;

			/* calculate extra gate delay */
			for(unsigned k = 0 ; k < vths.size() ; ++k)
			{
				double vth = vths[k];
				double extraDelay;
				if(vdd > 0.7)
					extraDelay = originalGateDelay * ( deltaVos / vdd - 1.5 * (deltaVis / (vdd - vth)) );
				else
					extraDelay = originalGateDelay * ( deltaVos / vdd + (2*1.294*(vdd - vth) - 1)/(0.1378*vth)*deltaVis  );
				extraGateDelays[k].push_back(extraDelay);
			}

			/* save transition , vls and vhs (for ideaResult) */
			int transitionID = extraGateDelays[0].size() - 1;
			
			// map transition location to ID
			transitionIDs[&transition] = transitionID;
			vls.push_back(vol);
			vhs.push_back(voh);
			assert(vls.size() == vhs.size() && vhs.size() == extraGateDelays[0].size());
		}
	}
	
	/* calculate max PSN-aware path delay*/
	vector<double> maxPsnAwarePathDelay(vths.size() , 0);
	getPsnAwarePathDelay(wave , cir_->po , maxPsnAwarePathDelay);
	getPsnAwarePathDelay(wave , cir_->ppo , maxPsnAwarePathDelay);

	/* calculate original path delay*/
	double originalMaxPathDelay;
	getMaxDelay(wave , originalMaxPathDelay);
	
	/* build idea result */
	IdeaResult ideaResult(originalMaxPathDelay , maxPsnAwarePathDelay , windowWidth , currentToVoltage);
	return ideaResult;
}

vector<matrixNs::Vector> &Idea::calculateCurrent(Wave *wave , double windowWidth)
{
	// circuit information
	const vector<Net> & nets = cir_->getNets();
	const vector<Cell> & cells = cir_->getCells();
	
	// obtain supply voltage
	PowerGrid *pg = pgs_->getPowerGrid();
	double supplyVoltage = pg->getSupplyVoltage();

	// using waves to generate current
	for(unsigned i = 0 ; i < nets.size() ; ++i)
	{
		int netID = i;
		int cellID = nets[i].ipt_cell_id;

		// there are no cells to drive this net ex: pi
		if(cellID == -1)
			continue;

		for(unsigned j = 0 ; j < wave[i].transition.size() ; ++j)
		{
			Transition &t = wave[i].transition[j];

			// calculate capacitance
			double capacitance = (t.value == Wave::H)?nets[i].totalRiseCap : nets[i].totalFallCap;

			// calculate switching charge (unit : p)
			double qsw = supplyVoltage*capacitance;
			
			/* calculate internal charge */

			// lookup library table to find internal power
			LookUpTableInfo info;
			info.returnType = INTERNAL_POWER;
			info.cellIndex = cells[cellID].type;
			info.pinIndex = cells[cellID].getOptPin(netID);
			assert(t.prevTransition);
			info.relativePinIndex = cells[cellID].getIptPin(t.prevTransition->netID);
			info.faninSignal = 0;
			info.toggleRise = t.value == Wave::H;
			info.capacitance = ( info.toggleRise )? nets[netID].totalRiseCap:nets[netID].totalFallCap;
			info.inputTransition = t.prevTransition->period;
			double internalPower = lib_->get(info);
			
			// internal charge (unit : p)
			double qin = internalPower*t.prevTransition->period/supplyVoltage;

			// power grid
			double qinqsw = qin + qsw;
			
			// get windows and portions
			vector<int> windows;
			vector<double> portions;
			getWindows(windowWidth , t , windows , portions);
			
			// calculate power grid charge flow of every window
			for(unsigned win = 0 ; win < windows.size() ; ++win)
			{
				int window = windows[win];
				double portion = portions[win];
				
				// calculate the charge flow of window
				double chargePortionQinqsw = qinqsw * portion;
				double chargePortionQin = qin * portion;
				
				// find vdd and gnd node of cell
				int vddNode = cellVDDNodeID[cellID];
				int gndNode = cellGNDNodeID[cellID];
				
				// add the charge flow
				// positive means current flow into the node
				// negative means current flow out the node
				if(t.value == Wave::H)
				{
					currentToVoltage[window].add(vddNode , -chargePortionQinqsw / windowWidth * 1e-3);
					currentToVoltage[window].add(gndNode , +chargePortionQin    / windowWidth * 1e-3);
				}
				else if(t.value == Wave::L)
				{
					currentToVoltage[window].add(vddNode , -chargePortionQin    / windowWidth * 1e-3);
					currentToVoltage[window].add(gndNode , +chargePortionQinqsw / windowWidth * 1e-3);
				}

			}
		}
	}
	return currentToVoltage;
}

