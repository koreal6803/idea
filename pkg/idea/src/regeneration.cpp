#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/stat.h>
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

#include "utility/src/tetra_max.h"
#include "re_atpg.h"


#include "verify.h"

#include "idea.h"
using namespace pgNs;
using namespace std;

vector<bool> identifyRiskPattern(std::string &log , double clock_period);
void remove(const string &dir)
{
	string script = "rm -r " + dir;
	system(script.c_str());
}
void mkdir(const string &dir)
{
	string script = "mkdir " + dir;
	system(script.c_str());
}

void move(const string &file1 , const string &file2)
{
	string script = "cp " + file1 + " " + file2;
	system(script.c_str());
}

bool allTrue(vector<bool> vec)
{
	for(unsigned i = 0 ; i < vec.size() ; ++i)
		if(!vec[i])
			return false;
	return true;
}
string toString(int cnt)
{
	stringstream ss;
	ss << cnt;
	return ss.str();
}

bool faultExist(const string &file)
{
	string in;
	ifstream fin(file.c_str());
	if(!getline(fin , in))
		return false;
	else
		return true;
}

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
int main(int argc , char ** argv)
{
	// check files
	if(argc < 4)
	{
		cout << "please input lib.v cir.v cirName pat.wgl idea.log methodID iteration" <<endl;
		return 0;
	}
	
	clock_t start,end;

	string libv = argv[1];
	string verilog = argv[2];
	string circuitName = argv[3];
	string wgl = argv[4];
	string idea = argv[5];
	int method = string(argv[6])[0] - '0';
	int iteration = string(argv[7])[0] - '0';

	writeProgramName();
	
	writeTitle("Input data");
	writeValue("library" , libv);
	writeValue("verilog" , verilog);
	writeValue("circuit" , circuitName);
	writeValue("wgl" , wgl);
	writeValue("idea log" , idea);
	writeValue("method" , method);
	writeValue("iteration" , iteration);
	cout << "continue...?" << endl;
	getchar();


	// ===============================================================================
	// pattern re-generation
	// ===============================================================================
	
	// setup workspace
	struct stat sb;
	if (stat("./reGene", &sb))
		mkdir("reGene");
	
	string reGeneWorkSpace = "./reGene/" + circuitName;
	if (stat(reGeneWorkSpace.c_str(), &sb))
		mkdir(reGeneWorkSpace);
	
	if(method == 0)
		reGeneWorkSpace = "./reGene/" + circuitName + "/method0";
	else
		reGeneWorkSpace = "./reGene/" + circuitName + "/method1";

	if (stat(reGeneWorkSpace.c_str(), &sb))
		mkdir(reGeneWorkSpace);

	// re-atpg generator
	ReAtpg ra;

	string targetWgl = wgl;

	// commercial tool
	TetraMax tmax;
	tmax.setVerilog(string("../../../../../") + verilog);
	tmax.setLibrary(string("../../../../../") + libv);
	tmax.setCircuit(circuitName);

	// set clock period
	double clock_period;
	if(circuitName == "leon3mp")
		clock_period = 2.1;
	else if(circuitName == "b19")
		clock_period = 3.7;
	else if(circuitName == "b18")
		clock_period = 3.6;
	else if(circuitName == "b17")
		clock_period = 2.1;
	else
		clock_period = 0;
		

	while(iteration < 10)
	{
		// in the iteration directory: 
		// pattern.wgl
		// safe.wgl
		// risk.wgl

		// 1-stage recoveryPat.wgl
		// 2-stage recoveryPat.wgl
		// 1-stage faultSim directory
		// 2-stage faultSim directory
		// 1-stage atpg directory
		// 2-stage atpg directory

		// create iteration directory
		string iterationDir = reGeneWorkSpace + "/iteration" + toString(iteration);
		
		// reset the iteration directory
		if (stat(iterationDir.c_str(), &sb) == 0)
			remove(iterationDir);
		mkdir(iterationDir);


		// copy wgl to pattern.wgl
		move(targetWgl , iterationDir + "/pattern.wgl");

		// identify the risky patterns
		vector<bool> safe = identifyRiskPattern(idea , clock_period);
		
		// stop while patterns are safe patterns
		if(allTrue(safe))
			break;
		
		// -- seperate wgl file to risk.wgl and safe.wgl

		// set risky patterns to re_atpg object
		for(unsigned i = 0 ; i < safe.size() ; ++i)
			if(!safe[i])
				ra.setRisk(i);
		
		// seperate targetWgl file into safe.wgl and risk.wgl under iterationDir directory
		if(!ra.writeSaveRiskWsl( targetWgl, iterationDir))
		{
			cout << "can't find wgl file: " << targetWgl << endl;
			exit(0);
		}

		// -- regenerate
		string fsDir = iterationDir + "/fs";
		mkdir(fsDir);


		string atpgDir = iterationDir + "/atpg";
		mkdir(atpgDir);

		
		string undetectFaultList;
		if(method == 0)
		{
			writeTime("1-stage fault simulation" , 
			undetectFaultList = ra.faultSim1Stage(fsDir , &tmax));
		}
		else
		{
			writeTime("2-stage fault simulation" , 
			undetectFaultList = ra.faultSim2Stage(fsDir , &tmax));
		}

		if(!faultExist(undetectFaultList))
			break;
		
		// -- atpg regnerate recovery patterns
		string recovery = ra.regene(atpgDir , &tmax , "../fs/undetect.flt");
		move(recovery + ".wgl" , iterationDir + "/recovery.wgl");

		// -- simulate recovery patterns
		
		// recovery stil to pat
		string script = "./fixStil/a.out " + recovery + ".stil > " + iterationDir + "/recovery.stil";
		system(script.c_str());
		
		script = "./script/stil2pat.py " + iterationDir + "/recovery.stil > " + iterationDir + "/recovery.pat";
		cout << script << endl;
		system(script.c_str());

		// simulate recovery patterns
		script = "./script/idea_log.sh " + circuitName + " 1.1 " 
				 + iterationDir + "/recovery.pat | tee " + iterationDir + "/recovery.log";
		system(script.c_str());
		
		// prepare next stage

		iteration++;
		targetWgl = iterationDir + "/recovery.wgl";
		idea = iterationDir + "/recovery.log";


	}
	return 0;
}

vector<bool> identifyRiskPattern(std::string &log , double clock_period)
{
	vector<bool> ret;

//|  IR aware Path Delay(vth=0.354)--------------------------3884
//|  Original Path Delay-----------------------------------2443.3
	ifstream fin(log.c_str());
	if(!fin)
	{
		cout << "**ERROR main: canot find file : " << log << endl;
		exit(0);
	}

	double maxPathDelay = 0;
	vector<double> psnAwarePathDelay;
	string line;
	while(getline(fin , line))
	{
		if(line[3] == 'I' && line.find("vth=0.354") != string::npos)
		{
			unsigned spos = line.find_last_of('-');
			line.erase(line.begin() , line.begin() + spos+1);
			stringstream ss;
			ss << line;
			double val;
			ss >> val;
			psnAwarePathDelay.push_back(val);
		}
		else if(line[3] == 'O' && line.find("Original") != string::npos)
		{
			unsigned spos = line.find_last_of('-');
			line.erase(line.begin() , line.begin() + spos+1);
			stringstream ss;
			ss << line;
			double val;
			ss >> val;
			maxPathDelay = max(val , maxPathDelay);
		}
	}
	writeValue("max original path delay" , maxPathDelay);
	if(clock_period == 0)
		clock_period = 1.07*maxPathDelay;
	writeValue("clock period" , clock_period);

	int cnt = 0;
	for(unsigned i = 0 ; i < psnAwarePathDelay.size() ; ++i)
		if(clock_period >= psnAwarePathDelay[i])
			ret.push_back(true);
		else
		{
			writeValue("Violated Pattern ID" , i+1);
			ret.push_back(false);
			cnt++;
		}
	writeValue("risky patterns" , cnt);
	return ret;
}
