// **************************************************************************
// File       [ idea_test.cpp ]
// Author     [ koreal6803 ]
// Synopsis   [ idea simulator for PSN-aware timing analysis ]
// Date       [ 2014/12/12 created ]
// **************************************************************************

/* standard template library */
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>

/* PSN-timing analysis library */
#include "core/src/library.h"
#include "core/src/library_parser.h"

#include "core/src/circuit_builder.h"
#include "core/src/circuit.h"

#include "core/src/net_capacitance_extracter.h"

#include "core/src/pattern_set.h"
#include "core/src/circuit_simulator.h"

#include "ir_core/src/power_grid.h"
#include "ir_core/src/power_grid_solver.h"

#include "power_info.h"
#include "verify.h"
#include "idea.h"

/* terminal output format */

#define writeProgramName()\
	cout << " ______________________________________________________________ " << endl;\
	cout << "|                                                              |" << endl;\
	cout << "|  NEW IDEA Simulator                      koreal6803          |" << endl;\
	cout << "|______________________________________________________________|" << endl;\
	cout << endl;

#define writeTitle(TITLE)\
	cout << "|" << endl;\
	cout << "|" << endl;\
	cout << "|" << endl;\
	cout << "|  " << TITLE << endl;\
	cout << "|______________________________________________________________ " << endl;\
	cout << "|" << endl;

#define writeTime(FUNC_NAME , FUNC)\
	cout << "|  " <<  std::left << setw(30)  << FUNC_NAME;\
	cout.flush();\
	start = clock();\
	FUNC;\
	end = clock();\
	cout << std::right << setw(29) << (double)(end - start)/CLOCKS_PER_SEC << "s"  <<endl;\


#define writeValue(NAME , VALUE)\
{\
	stringstream ss;\
	stringstream ss2;\
	ss << NAME;\
	ss2 << VALUE;\
	cout << "|  " << std::left << setw(30) << ss.str()  << std::right << setw(30) << ss2.str() <<endl;\
}


/* power grid namespace */
using namespace pgNs;

/* standard library namespace */
using namespace std;

/* check the parsing flag */
void check(bool success);

/* report error */
void reportError(VerifyIdea &verify);


int main(int argc , char ** argv)
{
	// check files
	if(argc < 7)
	{
		cout << "file format incorrect!" << endl;
		cout << "./idea_test.out lib vlog pat dspf spf vdd" << endl;
		return 0;
	}
	char *libraryFile = argv[1];
	char *vlogFile 	  = argv[2];
	char *patFile 	  = argv[4];
	char *dspfFile 	  = argv[5];
	char *spfFile 	  = argv[6];
	char *vddValue	  = argv[7];
	
	/* initial terminal output format */
	clock_t start,end;
	bool readSuccess = false;
	
	/* terminal output title */
	cout.fill('-');
	writeProgramName();
	
	// ================================================================================
	// cell manager contain nangate45 library information(pin name and cell type names)
	// ================================================================================
	
	/* print library informations */ 
	writeTitle("Parsing Library");

	/* initial library */ 
	Library lib;
	LibraryParser libp(&lib);

	/* parse library */
	writeValue("Library file", libraryFile);
	writeTime("Building library"  , readSuccess = libp.read(libraryFile));
	check(readSuccess);

	/* read extra delay library */
	writeTime("Building delay" , readSuccess = lib.readDelay("../../hspice/libMake/delays/v"+string(vddValue)+".delay"));
	check(readSuccess);
	
	// ===============================================================================
	// use circuit builder to build a circuit
	// ===============================================================================

	writeTitle("Parsing Circuit")
	
	/* circuit builder is used to parse and build the circuit */
	CircuitBuilder cb;
	
	/* tell circuit builder to build the circuit using cells in cell manager */
	cb.set(&lib);
	
	/* tell circuit builder to build the levelized circuit whose clk is set to level 0 */
	cb.set("CK");

	/* set flip flop functional pins */
	cb.set("Q","QN","D","SI","SE","CK");
	
	/* read the circuit */
	writeValue("Circuit file",vlogFile);
	writeTime( "read " , readSuccess = cb.read(vlogFile));
	check(readSuccess);

	/* get the circuit */
	Circuit cir = cb.getCircuit();

	/* print circuit information */
	writeTitle("Circuit Informations")
	writeValue("Cell amount",cir.cells.size());
	writeValue("Net amount",cir.nets.size());
	writeValue("Flip Flop amount",cir.ff.size());
	writeValue("Max Level",cir.cells.back().level);
	

	// ===============================================================================
	// Net Capacitance setup
	// ===============================================================================
	
	writeTitle("Parse Dspf & Spf to read net capacitance");

	/* declare extracter to a spicific circuit */
	NetCapExtracter spf(&cir);

	/* read net capacitance from spf file (input capacitance) */
	writeValue("SPF file", spfFile);
	writeTime( "read spf ", readSuccess = spf.read(spfFile));
	check(readSuccess);

	/* read net capacitance from dspf file (i/o pin capacitance) */
	writeValue("DSPF file", dspfFile);
	writeTime( "read dspf " , readSuccess = spf.read(dspfFile));
	check(readSuccess);
	
	// ===============================================================================
	// use pattern set to read the pattern
	// ===============================================================================
	
	writeTitle("Read Pattern Set")

	/* initial pattern set */
	PatternSet ps;
	
	/* tell pattern set to build ppi and pi order according to cell ID in the circuit */
	ps.set(&cir);
 
	/* setting pat file */
	writeValue("PAT file",patFile);
	writeTime("Read Pattern " , readSuccess = ps.setFile(patFile));
	check(readSuccess);
	
	/* read all the pattern at once instead of read pattern one by one */
	ps.readAll();
	writeValue("test length",ps.patterns.size());

	// ===============================================================================
	// use circuit simulator to simulate the circuit
	// ===============================================================================
	
	writeTitle("Circuit Simulator Setup");
	
	CircuitSimulator cs;
	
	/* tell which circuit will be simulated */
	cs.set(&cir);
	
	/* set cell manager to get pin name when simulation */
	cs.set(&lib);

	/* set clk wave of the simulation */
	Wave clkWave;
	clkWave.initialValue = Wave::L;

	/* set rise transition of clk */
	Transition riseTrans;
	riseTrans.value = Wave::H;
	riseTrans.time = 0.01;
	riseTrans.period = 0.0001;
	riseTrans.prevTransition = NULL;
	clkWave.addTransition(Wave::H , riseTrans);//0.0 , 0.002, NULL, 0);
	
	/* set fall transition of clk */
	Transition fallTrans;
	fallTrans = riseTrans;
	fallTrans.value = Wave::L;
	fallTrans.time = 0.25;
	clkWave.addTransition(Wave::L , fallTrans);//0.25 , 0.002, NULL, 0);

	/* clk of simulation initialization information */
	writeValue("Clk Rise Time",0.01);
	writeValue("Clk Fall Time",0.25);
	writeValue("Clk Transition",0.01);
	
	/* add clk wave to circuit simulator */
	cs.set("CK", &clkWave , 1.0);

	/* set pattern pi and ppi order */
	cs.set(&ps.piOrder , &ps.poOrder , &ps.ppiOrder);

	// ===============================================================================
	// power grid setup
	// ===============================================================================
	
	writeTitle("Build Power Grid");

	/* declare power grid */
	PowerGrid pg;

	/* read power grid */
	writeTime("read dspf ", readSuccess = pg.read(dspfFile));
	check(readSuccess);

	/* conver vddValue to double */
	stringstream ss(vddValue);

	double vdd;
	ss >> vdd;

	writeValue("Set VDD",vdd);
	
	/* power grid supply voltage setting */
	pg.addVDD(vdd);
	pg.addGND(0);

	writeValue("Node size",pg.getNodes().size());

	// ===============================================================================
	// power grid solver setup
	// ===============================================================================
	
	/* print the detail */
	writeTitle("Power Grid Solver Matrix");

	/* don't apply euler method */
	bool onlyR = true;
	writeValue("Apply Euler method",!onlyR);

	/* build matrix	*/
	writeTime("building matrix" , PowerGridSolver pgs(&pg,onlyR));

	// ===============================================================================
	// IDEA setup
	// ===============================================================================
	Idea idea;

	/* set power grid */
	idea.set(&pgs);

	/* set clk cycle */
	idea.set(2);

	/* set circuit */
	idea.set(&cir);

	/* set library */
	idea.set(&lib);
	
	// ===============================================================================
	// verify idea setup
	// ===============================================================================
	
	VerifyIdea verify;
	verify.set(&lib);
	verify.set(&idea);

	// ===============================================================================
	// Vth variation setting
	// ===============================================================================

	writeTitle("Vth Variation");

	writeValue("vth",0.354 - 0.2);
	writeValue("vth",0.354);
	writeValue("vth",0.354 + 0.2);

	vector<double> vths;
	vths.push_back(0.354 - 0.2);
	vths.push_back(0.354);
	vths.push_back(0.354 + 0.2);
	
	// ===============================================================================
	// setting power Info to calculate wsa fftc harzard ... etc
	// ===============================================================================
	PowerInfo powerInfo;

	// ===============================================================================
	// perform circuit simulation
	// ===============================================================================
	
	for(unsigned i = 0 ; i < ps.patterns.size() ; ++i)
	{
		Pattern pat = ps.patterns[i];
		
		cs.initial(pat.pis[0] , pat.ppi);
		
		bool launchClk  = pat.cycle[0] == Pattern::CAPTURE;
		bool captureClk = pat.cycle[1] == Pattern::CAPTURE;
		
		// pi start transition time
		double piTransitionTime = 0.5;
		
		// show the informations
		writeTitle("Pattern " << i);
		writeValue("Launch cycle"  ,launchClk);
		writeValue("Capture cycle" ,captureClk);

		// show (and perform) the time-logic simulation runtime
		writeTime("circuit simulation",cs.simulate(pat.pis[1] , piTransitionTime , launchClk));
		
		bool success = cs.checkValue(pat.pos[1], pat.ppo , captureClk);

		if(!success)
			writeValue("**WARRN Idea_test.cpp","can't match PPO2 with pat file");

		Wave *wave = cs.getWave();
		
		// calculate and output information of power analysis
		writeTime ("IDEA analysys",        IdeaResult ideaResult(idea.run(wave , vths)));
		writeValue("Window Size",          ideaResult.windowSize);
		writeValue("Window Amount",        ideaResult.voltage.size());
		writeValue("WSA",                  powerInfo.wsa(&idea));
		writeValue("FFTC",                 powerInfo.fftc(wave , &cir));
		writeValue("avg IR",               powerInfo.avgIR(&idea));
		writeValue("max IR",               powerInfo.maxIR(&idea));
		writeValue("avg Harzard",          powerInfo.avgHazard(wave , &cir));
		writeValue("max Harzard",          powerInfo.maxHazard(wave , &cir));
		writeValue("Original Path Delay",  ideaResult.originalPathDelay);

		for(unsigned j = 0 ; j < ideaResult.powerAwarePathDelay.size() ; ++j)
			writeValue("IR aware Path Delay(vth="<< vths[j] << ")", ideaResult.powerAwarePathDelay[j]);
		
		// verify workspace setup
		string workspace = (string(argv[7])=="1.1")?
		"../lady_idea/verify/circuit/"+cir.name:
		"../lady_idea/verify/circuit/"+cir.name+"_"+argv[7];

		// verify
		verify.verify(cir, wave, i, workspace);
	}
	reportError(verify);
	return 0;
}

/* check the parsing flag */
void check(bool success)
{
	if(!success)
	{
		cout << endl;
		cout << "*ERROR: read file fail!" << endl;
		exit(0);	
	}
}

void reportError(VerifyIdea &verify)
{
	double avgError(0);
	double maxError(0);

	double avgExtraError(0);
	double maxExtraError(0);

	double avgPortion(0);
	double maxPortion(0);

	double avgErrorN(0);
	double maxErrorN(0);

	double avgExtraErrorN(0);
	double maxExtraErrorN(0);

	for(unsigned i = 0 ; i < verify.ideaPathDelay.size() ; ++i)
	{
		double ipd = verify.ideaPathDelay[i];
		double ipdIR = verify.ideaPathDelayIR[i];
		double spd = verify.spicePathDelay[i];
		double spdIR = verify.spicePathDelayIR[i];
		if(spd == 0 && ipd == 0)
			continue;
		if(spd == spdIR)
			continue;
		writeTitle("Pattern" << i);
		writeValue("IDEA  total path delay" , ipd + ipdIR);
		writeValue("SPICE total path delay" , spd + spdIR);
		writeValue("IDEA  extra path delay" , ipdIR);
		writeValue("SPICE extra path delay" , spdIR);
		double error = (ipdIR - spdIR) / spdIR;
		double extraError = ( (ipdIR - ipd) - (spdIR - spd) ) / (spdIR - spd);
		double portion = abs(spdIR - spd)/ spdIR;

		// max error
		maxError = max(abs(error) , maxError);
		maxExtraError = max(abs(extraError), maxExtraError);
		maxPortion = max(abs(portion),maxPortion);
		maxErrorN = max(abs(ipdIR - spdIR) , maxErrorN);
		maxExtraErrorN = max(abs(ipdIR - ipd - spdIR + spd) , maxExtraErrorN);

		// average error
		avgError += abs(error);
		avgExtraError += abs(extraError);
		avgPortion += abs(portion);
		avgErrorN += abs(ipdIR - spdIR);
		avgExtraErrorN += abs(ipdIR - ipd - spdIR + spd );

	}
	avgError/=verify.ideaPathDelay.size();
	avgExtraError/=verify.ideaPathDelay.size();
	avgPortion/=verify.ideaPathDelay.size();
	avgErrorN/=verify.ideaPathDelay.size();
	avgExtraErrorN/=verify.ideaPathDelay.size();

	writeTitle("Error report");
	writeValue("average error" , avgError);
	writeValue("max error" , maxError);

	writeValue("average extra error" , avgExtraError);
	writeValue("max extra error" , maxExtraError);
}
