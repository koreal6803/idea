#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
//#include <omp.h>
#include "core/src/library.h"
#include "core/src/library_parser.h"

#include "core/src/circuit_builder.h"
#include "core/src/circuit.h"

#include "core/src/net_capacitance_extracter.h"

#include "core/src/pattern_set.h"
#include "core/src/circuit_simulator.h"

#include "ir_core/src/power_grid.h"
#include "ir_core/src/power_grid_solver.h"

#include "spice_simulator.h"

#include "verify.h"

#include "idea.h"
using namespace pgNs;
using namespace std;

#include <sys/stat.h>
void mkdir(const string &dir)
{
	string script = "mkdir " + dir;
	system(script.c_str());
}

int main(int argc , char ** argv)

{
	// check files
	if(argc < 10)
	{
		return 0;
	}
	
	cout << " ___________________________________________________ " << endl;
	cout << "|                                                   |" << endl;
	cout << "|  NEW IDEA Simulator            koreal6803         |" << endl;
	cout << "|___________________________________________________|" << endl;
	cout << endl;

	// ================================================================================
	// cell manager contain nangate45 library information(pin name and cell type names)
	// ================================================================================
	
	// print something
	cout << "|" << endl;
	cout << "|  Read Library" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  Read pattern from "<< endl;
	cout << "|  " << argv[1] << "...";
	cout.flush();

	// parse library
	Library lib;
	LibraryParser libp(&lib);
	libp.read(argv[1]);

	// print something
	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;
	
	// ===============================================================================
	// use circuit builder to build a circuit
	// ===============================================================================
	
	cout << "|" << endl;
	cout << "|  Read Circuit" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  Read pattern from "<< endl;
	cout << "|  " << argv[2] << "...";
	
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

	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;

	// get the circuit
	Circuit cir = cb.getCircuit();

	// print circuit information
	const vector<Cell> cells = cir.getCells();
	const vector<Net> nets = cir.getNets();
	cout.fill('-');
	cout << "|" << endl;
	cout << "|  Circuit informations" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  " << std::left << setw(24) << "circuit cells "  << std::right << setw(20) << cells.size()       <<endl;
	cout << "|  " << std::left << setw(24) << "circuit nets "   << std::right << setw(20) << nets.size()        <<endl;
	cout << "|  " << std::left << setw(24) << "flip flop size " << std::right << setw(20) << cir.ff.size()      <<endl;
	cout << "|  " << std::left << setw(24) << "maxmum level "   << std::right << setw(20) << cells.back().level <<endl;
	cout << "|" << endl;
	cout << endl << endl;

	// ===============================================================================
	// Net Capacitance setup
	// ===============================================================================
	
	cout << "|" << endl;
	cout << "|  Net Capacitance Extractor" << endl;
	cout << "|___________________________________________________ " << endl;

	// declare extracter to a spicific circuit
	NetCapExtracter spf(&cir);

	// print detail
	cout << "|" << endl;
	cout << "|  Read Net Capacitance from " <<endl;
	cout << "|  " << argv[6] << "...";

	// read net capacitance from spf file
	spf.read(argv[6]);

	// print detail
	cout << "done" << endl;
	cout << "|  " << endl;
	cout << "|  Read Net Capacitance from " << endl;
	cout << "|  " << argv[5] << "...";

	// read net capacitance from dspf file (i/o pin capacitance)
	spf.read(argv[5]);

	// print somethings
	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;
	
	// ===============================================================================
	// use pattern set to read the pattern
	// ===============================================================================
	
	// print the detail
	cout << "|" << endl;
	cout << "|  Read Pattern Set" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  Read pattern from "<< endl;
	cout << "|  " << argv[4] << "...";
	
	// initial pattern set
	PatternSet ps;
	
	// tell pattern set to build ppi and pi order according to cell ID in the circuit
	ps.set(&cir);
	
	// read the pattern file
	if(!ps.setFile(argv[4]))
	{
		cout << "read pattern file fail" <<endl;
		return 0;
	}

	// read all the pattern at once instead of read pattern one by one
	ps.readAll();

	// print something
	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;

	// ===============================================================================
	// use circuit simulator to simulate the circuit
	// ===============================================================================
	
	// print the detail
	cout << "|" << endl;
	cout << "|  Circuit simulator setup" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	
	CircuitSimulator cs;
	
	// tell which circuit will be simulated
	cs.set(&cir);
	
	// set the delay infomation
	//cs.set(&di);
	
	// set cell manager to get pin name when simulation
	cs.set(&lib);

	// set clk wave of the simulation
	Wave clkWave;
	clkWave.initialValue = Wave::L;

	// set rise transition of clk
	Transition riseTrans;
	riseTrans.value = Wave::H;
	riseTrans.time = 0.01;
	riseTrans.period = 0.0001;
	riseTrans.prevTransition = NULL;
	clkWave.addTransition(Wave::H , riseTrans);//0.0 , 0.002, NULL, 0);
	
	// set fall transition of clk
	Transition fallTrans;
	fallTrans = riseTrans;
	fallTrans.value = Wave::L;
	fallTrans.time = 0.25;
	clkWave.addTransition(Wave::L , fallTrans);//0.25 , 0.002, NULL, 0);
	
	cout << "|  " << std::left << setw(24) << "clk rise time: "  << std::right << setw(20) << 0.01       <<endl;
	cout << "|  " << std::left << setw(24) << "clk fall time: "  << std::right << setw(20) << 0.25       <<endl;
	cout << "|  " << std::left << setw(24) << "clk transition period: "  << std::right << setw(20) << 0.01       <<endl;
	cout << "|" << endl;
	cout << endl << endl;
	
	// add clk wave to circuit simulator
	cs.set("CK", &clkWave , 1.0);

	// set pattern pi and ppi order
	cs.set(&ps.piOrder , &ps.poOrder , &ps.ppiOrder);

	// ===============================================================================
	// power grid setup
	// ===============================================================================
	
	// print the detail
	cout << "|" << endl;
	cout << "|  Power Grid setup" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  Read pattern from "<< endl;
	cout << "|  " << argv[5] << "...";
	cout.flush();

	// declare power grid
	pgNs::PowerGrid pg;
	if(!pg.read(argv[5]))
	{
		cout << "fail" << endl;
		return 0;
	}

	// print success
	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;
	
	// seting vdd and gnd
	stringstream ss;
	ss << argv[7];
	double vdd;
	ss >> vdd;

	pg.addVDD(vdd);
	pg.addGND(0);

	// ===============================================================================
	// power grid solver setup
	// ===============================================================================
	
	// print the detail
	cout << "|" << endl;
	cout << "|  Power Grid Solver setup" << endl;
	cout << "|___________________________________________________ " << endl;
	cout << "|" << endl;
	cout << "|  Build matrix"<< endl;
	bool onlyR = true;
	if(onlyR)
		cout << "|  Without inductance and capacitance...";
	else
		cout << "|  Apply Euler method...";
		
	PowerGridSolver pgs(&pg,onlyR);
	cout << "done" << endl;
	cout << "|" << endl;
	cout << endl << endl;

	string workspace = string(argv[9]);
	// setup workspace
	struct stat sb;
	if (stat(workspace.c_str(), &sb))
		mkdir(workspace);
	
	workspace = string(argv[9]) + "/" + cir.name + "_" + string(argv[7]);
	if (stat(workspace.c_str(), &sb))
		mkdir(workspace);

	// ===============================================================================
	// perform circuit simulation
	// ===============================================================================
	SpiceSimulator pathGene(&cir, vdd);
	pathGene.set(&pg);
	pathGene.set(&ps.piOrder , &ps.poOrder , &ps.ppiOrder);
	pathGene.set(workspace);
	pathGene.setOriginalSpFile(string(argv[10]));
	
	//#pragma omp parallel for firstprivate(cs) 
	//

	SpiceSimulator::SimType type;
	cout << argv[8] << endl;
	if(string(argv[8]) == "HSPICE")
		type = SpiceSimulator::HSPICE;
	else
		type = SpiceSimulator::NANO_SIM;
	
	
	
	for(unsigned i = 0 ; i < ps.patterns.size() ; ++i)
	{
		Pattern &pat = ps.patterns[i];

		if(!pathGene.extractResult(type , workspace , i))
		{
			cout << "re-run pattern " << i << endl;
			if(!pathGene.sim(&pat, 0.5 , 0.01, type,i))
			{
				cout << "**ERROR: simulation fail!" << endl;
				continue;
			}
			pathGene.extractResult(type ,workspace, i);
		}

		
		stringstream ss;
		if(type == SpiceSimulator::HSPICE)
			ss << workspace << "/pat" << i << "/sp_result.txt";
		else
			ss << workspace << "/pat" << i << "/nano_result.txt";
		cout << ss.str() << endl;
		pathGene.save(ss.str());
	}
	return 0;
}

