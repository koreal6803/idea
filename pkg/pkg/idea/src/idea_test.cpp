#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "core/src/library.h"
#include "core/src/library_parser.h"

#include "core/src/circuit_builder.h"
#include "core/src/circuit.h"

#include "core/src/delay_info_builder.h"
#include "core/src/delay_info.h"

#include "core/src/pattern_set.h"
#include "core/src/circuit_simulator.h"

#include "ir_core/src/power_grid.h"
#include "ir_core/src/power_grid_solver.h"
#include "idea.h"
using namespace pgNs;
using namespace std;
double findMaxExtraDelay(Wave *wave , Circuit &cir);

int main(int argc , char ** argv)
{
	// set timer
	clock_t t1, t2;
	t1 = clock();

	// check files
	if(argc < 4)
	{
		cout << "please input verlog, sdf and pat file" <<endl;
		return 0;
	}

	// ================================================================================
	// cell manager contain nangate45 library information(pin name and cell type names)
	// ================================================================================
	Library lib;
	LibraryParser libp(&lib);
	libp.read(argv[1]);
	
	// ===============================================================================
	// use circuit builder to build a circuit
	// ===============================================================================
	
	CircuitBuilder cb;
	
	// tell circuit builder to build the circuit using cells in cell manager
	cb.set(&lib);
	
	// tell circuit builder to build the levelized circuit whose clk is set to level 0
	cb.set("CK");

	// set flip flop functional pins
	cb.set("Q","QN","D","SI","SE","CK");
	
	// read the circuit
	if(!cb.read(argv[2]))
	{
		cout << "read verlog file fail" <<endl;
		return 0;
	}

	// get the circuit
	Circuit cir = cb.getCircuit();
	
	// ===============================================================================
	// use delay info builder to build the delay info from sdf file
	// ===============================================================================
	cout << "read sdf file" <<endl;
	DelayInfoBuilder dib;
	
	// tell delay info builder to build the delay according to the circuit
	dib.set(&cir);
	
	// tell delay info builder to use cell manager to match the pin and nets
	dib.set(&lib);
	
	// read delay info
	if(!dib.read(argv[3]))
	{
		cout << "read sdf file fail" <<endl;
		return 0;
	}

	// get delay info
	DelayInfo di = dib.getDelayInfo();

	// ===============================================================================
	// use pattern set to read the pattern
	// ===============================================================================
	cout << "read pattern" <<endl;
	PatternSet ps;
	
	// tell pattern set to build ppi and pi order according to cell ID in the circuit
	ps.set(&cir);
	
	// read the pattern file
	if(!ps.setFile(argv[4]))
	{
		cout << "read pattern file fail" <<endl;
		return 0;
	}
	//ps.readAll();

	// ===============================================================================
	// use circuit simulator to simulate the circuit
	// ===============================================================================
	
	CircuitSimulator cs;
	
	// tell which circuit will be simulated
	cs.set(&cir);
	
	// set the delay infomation
	cs.set(&di);
	
	// set cell manager to get pin name when simulation
	cs.set(&lib);

	// set clk wave of the simulation
	Wave clkWave;
	clkWave.initialValue = Wave::L;
	clkWave.addTransition(Wave::H , 0.0 , 0.002, NULL, 0);
	clkWave.addTransition(Wave::L , 0.25 , 0.002, NULL, 0);
	cs.set("CK", &clkWave , 1.0);

	// set pattern pi and ppi order
	cs.set(&ps.piOrder , &ps.poOrder , &ps.ppiOrder);
	// ===============================================================================
	// power grid setup
	// ===============================================================================
	PowerGrid pg;
	pg.read(argv[5]);
	pg.addVDD(1.1);
	pg.addGND(0);
	// ===============================================================================
	// power grid solver setup
	// ===============================================================================
	bool onlyR = true;
	PowerGridSolver pgs(&pg,onlyR);
	// ===============================================================================
	// IDEA setup
	// ===============================================================================
	Idea idea;
	idea.set(&pgs);
	idea.set(2);
	idea.set(&di);
	idea.set(&cir);
	// ===============================================================================
	// perform circuit simulation
	// ===============================================================================
	cout << "start!" << endl;
	int cnt(0);
	//#pragma omp parallel for firstprivate(cs) 
	//for(unsigned i = 0 ; i < ps.patterns.size() ; ++i)
	while(ps.readNextPattern())
	{
		Pattern pat = ps.getPattern();
		
		cs.initial(pat.pis[0] , pat.ppi);
		
		bool launchClk  = pat.cycle[0] == Pattern::CAPTURE;
		bool captureClk = pat.cycle[1] == Pattern::CAPTURE;
		
		double piTransitionTime = 0.5;
		cs.simulate(pat.pis[1] , piTransitionTime , launchClk);
		
		bool success = cs.checkValue(pat.pos[1], pat.ppo , captureClk);
		Wave *wave = cs.getWave();
		idea.run(wave);
		// #pragma omp critical
		

		for(unsigned i = 0 ; i < cir.getNets().size() ; ++i)
			for(unsigned j = 0 ; j < wave[i].size ; ++j)
				wave[i].transition[j].netID = i;

		double extra_delay = findMaxExtraDelay(cs.getWave() , cir);

		cout <<  cnt++ << "th\t patterns is done!" <<  "\tcheck: " << success;
		cout << "\textra path delay: " << extra_delay << endl;
		
		if(extra_delay > 100 || extra_delay < -100)
			return 0;
		/*
		cout << "time step info" << endl;
		for(unsigned i = 0 ; i < idea.current.size() ; ++i)
		{
			if(idea.voltage[i][0] == -1)
				continue;
			cout << i << "th time step\t===========================" << endl;
			cout << "\tperiod: " << i*idea.maxTransitionPeriod << " to " << (i+1)*idea.maxTransitionPeriod << endl;
			cout << "\tNodes" << endl;
			// each node
			for(unsigned j = 0 ; j < idea.current[i].size() ; ++j)
			{
				if(pg.getNodes()[j].getName().substr(0,3) == "VDD" || pg.getNodes()[j].getName().substr(0,3) == "VSS")
					continue;
				cout << "\t================================="<<endl;
				cout << "\tpower grid node: " << pg.getNodes()[j].getName() << endl;
				cout << "\taverage current: " << idea.current[i][j] << endl;
				cout << "\taverage voltage: " << idea.voltage[i][j] << endl;
				cout << "\ttranstions     : " << endl;
			}
			cout << "\tTransitions" << endl;
			for(unsigned j = 0 ; j < idea.stepTransitions[i].size(); ++j)
			{
				cout << "\t================================="<<endl;
				cout << "\ttime  : " << idea.stepTransitions[i][j]->time << endl;
				cout << "\tperiod: " << idea.stepTransitions[i][j]->period << endl;
				cout << "\trise  : " << idea.stepTransitions[i][j]->value << endl;
			}
		}
		cout << "first pattern======================" << endl;
		for(unsigned i = 0 ; i < cir.getNets().size(); ++i)
		{
			cout << "    ===============================" << endl;
			cout << "    Net name        : " << cir.getNets()[i].name << endl;
			cout << "    Total Transition: " << wave[i].size << endl;
			cout << "    Rise Cap load   : " << cir.getNets()[i].totalRiseCap << endl;
			cout << "    Fall Cap load   : " << cir.getNets()[i].totalFallCap << endl;
			for(unsigned j = 0 ;j < wave[i].size ; j++)
			{
			cout << "        wave------------------------" << endl;
			cout << "        time         : " << wave[i].transition[j].time << endl;
			cout << "        period       : " << wave[i].transition[j].period << endl;
			cout << "        value        : " << wave[i].transition[j].value << endl;
			cout << "        vdd          : " << wave[i].transition[j].vh << endl;
			cout << "        vss          : " << wave[i].transition[j].vl << endl;
			cout << "        delay        : " << wave[i].transition[j].gateDelay << endl;
			cout << "        extra delay  : " << wave[i].transition[j].extraGateDelay << endl;
			}
		}
		}*/
		
	}
	//}
	t2 = clock();
	cout << "total cost: " <<  (t2-t1)/(double)(CLOCKS_PER_SEC) << " second!" <<endl;

	//ofstream fout;
	//fout.open("time.txt",ios::app);
	//fout << cir.getName() << ","  << cir.getNets().size() << "," << ps.patterns.size()<< ","<< (t2-t1)/(double)(CLOCKS_PER_SEC)/12 << endl;
	return 0;
}

double findMaxExtraDelay(Wave *wave , Circuit &cir)
{
	int latestWave = 0;
	double time = 0;
	double waveSize = cir.getNets().size();
	for(unsigned i = 0  ; i < waveSize; ++i)
	{
		if(wave[i].size == 0)
			continue;
		if(time < wave[i].transition[wave[i].size-1].time)
		{
			time = wave[i].transition[wave[i].size-1].time;
			latestWave = i;
		}
	}
	double ret(0);
	const Transition *trans = &wave[latestWave].transition[wave[latestWave].size-1];
	const vector<Net> nets = cir.getNets();
	const vector<Cell> cells = cir.getCells();
	while(trans != NULL)
	{
		int netID = trans->netID;
		cout << "trans = " << trans << "\t" << "extra gate delay: " << trans->extraGateDelay << " vd: " << trans->vh << " gn: " << trans->vl << " d: " << trans->gateDelay << " v: " << trans->value  << "netID: " << netID;
		if(netID != -1)
			cout << " netName: " << nets[netID].name;
		if(nets[netID].ipt_cell_id != -1)
			cout << " cellID: " << nets[netID].ipt_cell_id << " cellName: " << cells[nets[netID].ipt_cell_id].name;
		cout << endl;
		ret += trans->extraGateDelay;
		trans = trans->prevTransition;
	}
	return ret;
}
