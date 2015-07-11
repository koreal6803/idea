#include "re_atpg.h"
#include "utility/src/tetra_max.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


ReAtpg::ReAtpg(){}
ReAtpg::~ReAtpg(){}


bool ReAtpg::setRisk(int riskPatID)
{
	riskPatIDs_.insert(riskPatID);
	return true;
}
string ReAtpg::faultSim1Stage(const string &reGeneWorkSpace , TetraMax *tmax)
{
	// change current path to re-generation workspace
	tmax->setWorkSpace(reGeneWorkSpace.c_str());

	// risky pat fault simulation
	tmax->setPatterns("../safe.wgl");
	tmax->setFaults("-all");
	tmax->setOutputFaultList("undetect.flt -class nd -replace -collapsed");
	tmax->fsim("sim.tcl");



	return reGeneWorkSpace + "/undetect.flt";
}
string ReAtpg::faultSim2Stage(const string &reGeneWorkSpace , TetraMax *tmax )
{
	// change current path to re-generation workspace
	tmax->setWorkSpace(reGeneWorkSpace.c_str());

	// risky pat fault simulation
	tmax->setPatterns("../risk.wgl");
	tmax->setFaults("-all");
	tmax->setOutputFaultList("risk.flt -class pt dt -replace -collapsed");
	tmax->fsim("sim1.tcl");

	// risky fault reduction
	tmax->setPatterns("../safe.wgl");
	tmax->setFaults("risk.flt");
	tmax->setOutputFaultList("undetect.flt -class nd -replace -collapsed");
	tmax->fsim("sim2.tcl");

	return reGeneWorkSpace + "/undetect.flt";
}

string ReAtpg::regene(const string &reGeneWorkSpace , TetraMax *tmax , const string &undetect)
{
	
	// change current path to re-generation workspace
	tmax->setWorkSpace(reGeneWorkSpace.c_str());

	// check if fault exist
		

	// atpgGeneration
	tmax->setFaults(undetect);
	tmax->setOutputWsl("newPat.wgl");
	tmax->setOutputStil("newPat.stil");
	tmax->atpg("atpg.tcl");
	
	return reGeneWorkSpace + string("/newPat");
}

string inputOutput;
bool ReAtpg::writeSaveRiskWsl(const string &wglFile , const string &reGeneWorkSpace)
{
	ifstream fin(wglFile.c_str());
	if(!fin)
	{
		cout << "ReAtpg::writeSaveRiskWsl: can't find wglFile :" << wglFile << endl;
		return false;
	}

	safeWglAddress = reGeneWorkSpace + "/safe.wgl";

	riskWglAddress = reGeneWorkSpace + "/risk.wgl";

	ofstream safePat(safeWglAddress.c_str());
	ofstream riskPat(riskWglAddress.c_str());
	
	string line;
	int patID = -1;
	int safePatID = -1;
	int riskPatID = -1;

	// head
	int scanID = 0;
	bool scanState = false;
	while(getline(fin , line))
	{
		if(line[0] == '#')
			continue;
		if(line.find("fast_sequential") != string::npos)
			break;
		if(!scanState && line.find("scanstate") != string::npos)
			scanState = true;
		if(scanState && line.find("chain") != string::npos && line.find(":=") != string::npos)
		{
			/*
			cout << scanID/2 << endl;
			cout << line << endl;
			getchar();
			*/
			if(riskPatIDs_.count(scanID/2) > 0)
			{
				while(*(line.end()-1) != ';')
				{
					riskPat << line << endl;
					getline(fin , line);
				}
				riskPat << line << endl;
			}
			else
			{
				while(*(line.end()-1) != ';')
				{
					safePat << line << endl;
					getline(fin , line);
				}
				safePat << line << endl;
			}
			scanID++;
		}
		else
		{
			safePat << line << endl;
			riskPat << line << endl;
		}
	}
	string realInputOutput;

	// pattern
	while(1)
	{
		if(line.find("fast_sequential") != string::npos)
			patID++;
		else
			break;// end
		if(riskPatIDs_.count(patID) > 0)
		{
   			riskPat << "   { pattern " << ++riskPatID << " fast_sequential }" << endl;
			line = writePat(riskPat , fin);
			realInputOutput = inputOutput;
		}
		else
		{
   			safePat << "   { pattern " << ++safePatID << " fast_sequential }" << endl;
			line = writePat(safePat , fin);
		}
	}
	
	riskPat << "{ end_pattern }"<<endl;
	riskPat << "end end" << endl;
	// foot
	do
	{
		safePat << line << endl;
   //output [chain1:chain1U9], input [chain1:chain1L9];
	}
	while(getline(fin , line));

	fin.close();
	riskPat.close();
	safePat.close();


	return true;
}

string ReAtpg::writePat(ofstream &pats , ifstream &all)
{
	string line;
	while(getline(all , line) && line.find("fast_sequential") == string::npos && line.find("end_pattern") == string::npos)
	{
		if(line.find("input")!=string::npos && line.find("output")!= string::npos)
			inputOutput = line;
		pats << line << endl;
	}
	return line;
}


void ReAtpg::cleanWorkSpace(const string &workSpace)
{
	string script = "rm -r " + workSpace;
	system(script.c_str());
	script = "mkdir " + workSpace;
	system(script.c_str());
}
