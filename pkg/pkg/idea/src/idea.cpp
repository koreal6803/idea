#include "idea.h"
#include "core/src/wave.h"
#include "core/src/circuit.h"
#include "core/src/delay_info.h"

#include "ir_core/src/power_grid.h"
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iomanip>
#include <iostream>

#ifdef USEDEBUG
#define DBG(x) std::cout << x
#define DBGF(x) x
#else
#define DBG(x) 
#define DBGF(x)
#endif

using namespace std;
using namespace pgNs;

Idea::Idea()
{
	cir_ = NULL;
	pgs_ = NULL;
	delayInfo = NULL;
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

void Idea::set(DelayInfo *delayInfo)
{
	this->delayInfo = delayInfo;
}

void Idea::run(Wave *wave)
{
	// find max path delay
	maxPathDelay = 0;
	double waveSize = cir_->getNets().size();
	const vector<Net> & nets = cir_->getNets();
	for(unsigned i = 0  ; i < waveSize; ++i)
	{
		if(wave[i].size == 0)
			continue;
		maxPathDelay = max(maxPathDelay , wave[i].transition[wave[i].size - 1].time);
	}
	
	// find max transition time
	maxTransitionPeriod = 0;
	for(unsigned i = 0 ; i < waveSize ; ++i)
		for(unsigned j = 0 ; j < wave[i].size ; ++j)
			maxTransitionPeriod = max(maxTransitionPeriod , wave[i].transition[j].period);
	
	DBG("max transition period: " << maxTransitionPeriod << endl);
	DBG("max path delay: " << maxPathDelay << endl);

	// idea simulation
	PowerGrid *pg = pgs_->getPowerGrid();
	vector<Node> &pgNodes = pg->getNodes();
	double timeStepSize = int(maxPathDelay/maxTransitionPeriod) + 2;
	double sourceVoltage = pg->getSupplyVoltage();
	
	current.assign(timeStepSize , vector<double>(pgNodes.size(),0));
	stepTransitions.assign(timeStepSize,vector<Transition*>());
	voltage.assign(timeStepSize , vector<double>(pgNodes.size(),-1));
	DBG( "time step amount: " << timeStepSize << endl);
	for(unsigned i = 0 ; i < waveSize ; ++i)
	{
		int cellID = nets[i].ipt_cell_id;
		DBG("net:    " << cir_->getNets()[i].name << endl);
		DBG("cellID: " << cellID << endl);
		if(cellID == -1)
			continue;
		DBG("cell:   " << cir_->getCells()[cellID].name << endl);
		for(unsigned j = 0 ; j < wave[i].size ; ++j)
		{
			Transition &t = wave[i].transition[j];
			int timeStep1 = (t.time - 0.5 * t.period)/maxTransitionPeriod;
			int timeStep2 = timeStep1 + 1;
			double capacitance = (t.value == Wave::H)?nets[i].totalRiseCap : nets[i].totalFallCap;
			double charge = sourceVoltage*capacitance*10e-3;
			double partialPeriod2 =(( t.time + 0.5 * t.period ) - (double)timeStep2 * maxTransitionPeriod);
			if(partialPeriod2 < 0)
				partialPeriod2 = 0;
			double partialPeriod1 = t.period - partialPeriod2;
			
			double partialCharge1 = partialPeriod1 / t.period * charge;
			double partialCharge2 = partialPeriod2 / t.period * charge;
			
			int node = (t.value == Wave::H)? cellVDDNodeID[cellID]:cellGNDNodeID[cellID];
			current[timeStep1][node] += partialCharge1/maxTransitionPeriod;
			current[timeStep2][node] += partialCharge2/maxTransitionPeriod;
			
			DBGF(
				cout << "wave===========" << endl;
				cout << "time:         " << t.time <<endl;
				cout << "period:       " << t.period << endl;
				cout << "total charge: " << charge << endl;
				cout << "time step1:   " << timeStep1 << "\tstart at: " << timeStep1 * maxTransitionPeriod << endl;
				cout << "time step2:   " << timeStep2 << "\tstart at: " << timeStep2 * maxTransitionPeriod << endl;
				cout << "time portion:      " << partialPeriod1 << ":" << partialPeriod2 << endl;
				cout << "charge:      " << partialCharge1 << ":" << partialCharge2 << endl;
				cout << "nodeID:      " << node << endl;
			);
			stepTransitions[timeStep1].push_back(&t);
			if(t.time + 0.5 * t.period > timeStep2 * maxTransitionPeriod)
				stepTransitions[timeStep2].push_back(&t);
		}
	}
	DBGF(
		cout << "transition cell" << endl;
		for(unsigned i = 0 ; i < current.size() ; ++i)
		{
			if(stepTransitions[i].size() == 0)
				continue;
			for(unsigned j = 0 ; j < current[i].size() ; ++j)
				pgNodes[j].setCurrent(current[i][j]);
			for(unsigned j = 0; j < pgNodes.size() ; j++)
			{
				if(pgNodes[j].isPad())
					cout << "PAD "; 
				else
					cout << "nod ";
				if(pgNodes[j].getType() == Node::POWER)
					cout << " power  network ";
				else
					cout << " ground network ";
				
				cout << setw(40) <<  pgNodes[j].getName() << setw(10) << setprecision(5) << pgNodes[j].getCurrent() <<endl;
			}
		}
	);
	for(unsigned i = 0 ; i < current.size() ; ++i)
	{
		if(stepTransitions[i].size() == 0)
			continue;
		for(unsigned j = 0 ; j < current[i].size() ; ++j)
			pgNodes[j].setCurrent(current[i][j]);
		pgs_->solve();
		for(unsigned j = 0; j < pgNodes.size() ; j++)
			voltage[i][j] = pgNodes[j].getVoltage();
		/*for(unsigned j = 0; j < pgNodes.size() ; j++)
		{
			if(pgNodes[j].isPad())
				cout << "PAD "; 
			else
				cout << "nod ";
			if(pgNodes[j].getType() == Node::POWER)
				cout << " power  network ";
			else
				cout << " ground network ";
			
			cout << setw(40) <<  pgNodes[j].getName() << setw(10) << setprecision(5) << "voltage: " << pgNodes[j].getVoltage()<< " current: " << pgNodes[j].getCurrent()  <<endl;
		}
		getchar();*/
	}
	
	
	for(unsigned i = 0 ; i < waveSize ; ++i)
	{
		int cellID = nets[i].ipt_cell_id;
		if(cellID == -1)
			continue;
		for(unsigned j = 0 ; j < wave[i].size ; ++j)
		{
			Transition &t = wave[i].transition[j];
			int timeStep1 = (t.time - 0.5 * t.period)/maxTransitionPeriod;
			int timeStep2 = timeStep1 + 1;
			double partialPeriod2 =(( t.time + 0.5 * t.period ) - (double)timeStep2 * maxTransitionPeriod);
			if(partialPeriod2 < 0)
				partialPeriod2 = 0;
			double partialPeriod1 = t.period - partialPeriod2;
			int vddNode = cellVDDNodeID[cellID];
			int gndNode = cellGNDNodeID[cellID];
			double vha = voltage[timeStep1][vddNode];
			double vhb = voltage[timeStep2][vddNode];
			double vla = voltage[timeStep1][gndNode];
			double vlb = voltage[timeStep2][gndNode];
			double vh = vha * partialPeriod1 / t.period + vhb * partialPeriod2 / t.period;
			double vl = vla * partialPeriod1 / t.period + vlb * partialPeriod2 / t.period;
			double transitionTime(0);
			t.vl = vl;
			t.vh = vh;
			if(t.prevTransition)
			{
				const Transition *prevT = t.prevTransition;
				transitionTime = prevT->period;
			}
			double deltaV = 1.1 - vh + vl;
			double extraDelay = deltaV / 1.1;
			if(wave[i].transition[j].value == Wave::L)
				extraDelay = -extraDelay;
			wave[i].transition[j].extraGateDelay = extraDelay;
			// cout << "gate delay: " << t.gateDelay <<" extra gate delay: " << extraDelay << endl;
		}
	}
}
