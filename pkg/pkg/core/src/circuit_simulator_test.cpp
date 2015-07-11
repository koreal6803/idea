#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <omp.h>
#include "library.h"
#include "library_parser.h"

#include "circuit_builder.h"
#include "circuit.h"

#include "delay_info_builder.h"
#include "delay_info.h"

#include "pattern_set.h"
#include "circuit_simulator.h"

using namespace std;

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
	ps.readAll();

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
	// perform circuit simulation
	// ===============================================================================
	cout << "start!" << endl;
	#pragma omp parallel for firstprivate(cs) 
	for(unsigned i = 0 ; i < ps.patterns.size() ; ++i)
	{
		Pattern pat = ps.patterns[i];
		
		cs.initial(pat.pis[0] , pat.ppi);

		bool launchClk  = pat.cycle[0] == Pattern::CAPTURE;
		bool captureClk = pat.cycle[1] == Pattern::CAPTURE;
		
		double piTransitionTime = 0.5;
		cs.simulate(pat.pis[1] , piTransitionTime , launchClk);

		bool success = cs.checkValue(pat.pos[1], pat.ppo , captureClk);
		#pragma omp critical
		{
		cout <<  i << "th\t patterns is done!" <<  " check: " << success <<endl;
		}
	}
	//}
	t2 = clock();
	cout << "total cost: " <<  (t2-t1)/(double)(CLOCKS_PER_SEC)/12 << " second!" <<endl;

	ofstream fout;
	fout.open("time.txt",ios::app);
	fout << cir.getName() << ","  << cir.getNets().size() << "," << ps.patterns.size()<< ","<< (t2-t1)/(double)(CLOCKS_PER_SEC)/12 << endl;
	return 0;
}
