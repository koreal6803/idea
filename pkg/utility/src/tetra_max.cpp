#include "tetra_max.h"
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

TetraMax::TetraMax(){}

void TetraMax::setWorkSpace(const string &workSpace)
{
	workSpace_ = workSpace;
}
void TetraMax::setVerilog(const string &verilog)
{
	verilog_ = verilog;
}
void TetraMax::setLibrary(const string &library)
{
	library_ = library;
}
void TetraMax::setCircuit(const string &circuit)
{
	circuit_ = circuit;
}
void TetraMax::setFaults(const string &faultList)
{
	faultList_ = faultList;
}
void TetraMax::setPatterns(const string &wsl)
{
	wsl_ = wsl;
}
void TetraMax::setOutputFaultList(const string &faultList)
{
	optFaultList_ = faultList;
}
void TetraMax::fsim(const string &simFileName)
{
	char currentPath[500];
	getcwd(currentPath , sizeof(currentPath));

	chdir(workSpace_.c_str());
	
	ofstream fout(simFileName.c_str());
	fout << "read_netlist " << verilog_ << endl;
	fout << "read_netlist " << library_ << endl;
	fout << "run_build_model " << circuit_ << endl;

	fout << "set time \"2 1 2 0\"" << endl;
	fout << "add_clock 0 CK -shift -timing $time" << endl;
	fout << "add_scan_enable 1 test_se" << endl;
	fout << "add_pi_constraints 0 test_se" << endl;
	fout << "add_scan_chain chain1 test_si test_so" << endl;
	fout << "set_drc -chain_trace chain1 -clock CK" << endl;
	fout << "run_drc" << endl;

	fout << "set_delay -launch system_clock" << endl;
	fout << "set_faults -model transition" << endl;
	
	if(faultList_ == "-all")
		fout << "add_faults " << faultList_ << endl;
	else
		fout << "read_faults " << faultList_ << endl;

	fout << "set_patterns -external " << wsl_ << endl;
	fout << "set_simulation -measure pat" << endl;
	fout << "run_fault_sim" << endl;
	fout << "write_faults "<< optFaultList_ << endl;
	
	string script = "tmax64 < " + simFileName + " | tee " + simFileName + ".log";
	system(script.c_str());

	chdir(currentPath);
}
void TetraMax::setOutputWsl(const string &wsl)
{
	optWsl_ = wsl;
}
void TetraMax::setOutputStil(const string &stil)
{
	optStil_ = stil;
}
void TetraMax::atpg(const string &fName)
{
	char currentPath[500];
	getcwd(currentPath , sizeof(currentPath));

	chdir(workSpace_.c_str());
	
	ofstream fout(fName.c_str());
	fout << "read_netlist " << verilog_ << endl;
	fout << "read_netlist " << library_ << endl;
	fout << "run_build_model " << circuit_ << endl;

	fout << "set time \"2 1 2 0\"" << endl;
	fout << "add_clock 0 CK -shift -timing $time" << endl;
	fout << "add_scan_enable 1 test_se" << endl;
	fout << "add_pi_constraints 0 test_se" << endl;
	fout << "add_scan_chain chain1 test_si test_so" << endl;
	fout << "set_drc -chain_trace chain1 -clock CK" << endl;
	fout << "run_drc" << endl;

	fout << "set_delay -launch system_clock" << endl;
	fout << "set_faults -model transition" << endl;
	
	if(faultList_ == "-all")
		fout << "add_faults " << faultList_ << endl;
	else
		fout << "read_faults " << faultList_ << endl;

	fout << "set_atpg -abort_limit 10 -merge high" << endl;
	fout << "set_atpg -capture_cycles 2" << endl;
	fout << "run_atpg -auto_compression" << endl;
	fout << "report_summaries" << endl;
	fout << "write_patterns " << optStil_ << " -format stil -parallel -nopatinfo" << endl;
	fout << "write_patterns " << optWsl_ << " -format wgl -parallel -nopatinfo" << endl;

	string script = "tmax64 < " + fName + " | tee " + fName + ".log";
	system(script.c_str());
	
	chdir(currentPath);
}
