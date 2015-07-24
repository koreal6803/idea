#include "spice_simulator.h"
#include "verify.h"
#include "idea.h"
#include "core/src/circuit.h"
#include "core/src/pattern_set.h"
#include "utility/src/spMaker.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <map>
using namespace std;
using namespace pgNs;

bool SpiceSimulator::set(vector<int> *piOrder , vector<int> *poOrder, vector<int> *ppiOrder)
{
	this->piOrder = piOrder;
	this->ppiOrder = ppiOrder;
	this->poOrder = poOrder;
	return true;
}

bool SpiceSimulator::set(pgNs::PowerGrid *pg)
{
	pg_ = pg;
	return true;
}

bool SpiceSimulator::set(std::string workspace)
{
	this->workspace = workspace;
	return true;
}

bool SpiceSimulator::setOriginalSpFile(const std::string &originalSpFile)
{
	this->originalSpFile = originalSpFile;
	return true;
}
bool SpiceSimulator::sim(Pattern *pat, double piTime, double piPeriod, SimType type,int no)
{
	stringstream ss;
	ss << no;
	string command = ("mkdir "+workspace+"/pat" + ss.str());
	system(command.c_str());
	string spFileName = workspace +"/pat"+ss.str()+"/pat.sp";
	std::string line;
	
	std::string fileName = originalSpFile;

	std::ifstream fin(fileName.c_str());
	if(!fin)
	{
		std::cout << "**WARRN VerifyIdea::verify: Can't find file " << fileName << std::endl;
		std::cout << "        simulation fail" << endl;
		return false;;
	}

	// create vdd string
	double vdd = pg_->getSupplyVoltage();
	cout << vdd << endl;
	stringstream s2;
	s2 << vdd;
	
	string vdd_s = s2.str();

	// copy the content of sp file
	SpMaker sp(spFileName);
	while(std::getline(fin , line))
	{
		//cout << line << endl;
		if(line[0] == 'V' && line.find("VDD_IDEAL")!=string::npos)
			sp.add("VDD_IDEAL VDD_NODE 0 " + vdd_s);
		else
			sp.add( line );

		if(line == "* INPUT")
			break;
	}

	
	// setup voltage source
	for(unsigned i = 0 ; i < pg_->vddNodes.size() ; ++i){
		string nodeName(pg_->getNodes()[pg_->vddNodes[i]].getName());
		size_t found = nodeName.find(".");
		assert(found != string::npos);
		nodeName[found] = '_';
		sp.add("VDDSOURCE " + nodeName + " 0 "+vdd_s+"v");
	}
	for(unsigned i = 0 ; i < pg_->gndNodes.size() ; ++i){
		string nodeName(pg_->getNodes()[pg_->gndNodes[i]].getName());
		size_t found = nodeName.find(".");
		assert(found != string::npos);
		nodeName[found] = '_';
		sp.add("VSSSOURCE " + nodeName + " 0 0v");
	}

	// setup PPI
	const vector<Cell> &cells = cir_->getCells();
	const vector<Net> &nets = cir_->getNets();
	string ffstring = "* flip-flop switch";
	sp.add( ffstring );
	int qPin = 0;//pinQ_;
	int qnPin = 1;//pinQN_;
	for(unsigned i = 0 ; i < pat->ppi.size() ; ++i)
	{
		double val = (pat->ppi[i] == '1')?vdd_:0;
		int cellID = ppiOrder->at(i);
		// print Q
		int netQ = cells[cellID].opt_net_id[qPin];
		if(netQ != -1){
			
			string qNode = cells[cellID].name + "_Q";
			double netVal = val;
			stringstream ss;
			ss << netVal;
			sp.add(".ic v("+qNode+")="+ss.str());
			if(val != 0){
				sp.add(".ic v(X"+cells[cellID].name+"."+"N_8_M8_s"+")="+vdd_s+"v");
				sp.add(".ic v(X"+cells[cellID].name+"."+"n_5_m20_g"+")="+vdd_s+"v");
			}
			else{
				sp.add(".ic v(X"+cells[cellID].name+"."+"N_8_M8_s"+")=0v");
				sp.add(".ic v(X"+cells[cellID].name+"."+"n_5_m20_g"+")=0v");
			}
		}

		// print QN
		int netQN = cells[cellID].opt_net_id[qnPin];
		if(netQN != -1){
			string qNode = cells[cellID].name + "_QN";
			double netVal = (val == vdd_)?0:vdd_;
			stringstream ss;
			ss << netVal;
			sp.add(".ic v("+qNode+")="+ss.str());
		}
	}
	sp.add("* PI switch");
	sp.add("VCK CK 0 PWL(0ns 0 0.009875ns 0 0.010125ns "+vdd_s+" 5ns "+vdd_s+")");
	double tranStartTime = piTime - piPeriod/2*5/2;
	double tranEndTime = piTime + piPeriod/2*5/2;
	// setup PI
	for(unsigned i = 0 ; i < pat->pis[0].size() ; ++i)
	{
		int netID = piOrder->at(i);
		string netName = nets[netID].name;

		if(netName == "CK")
			continue;
		
		double v1 = (pat->pis[0][i] == '1')?vdd_:0;
		double v2 = (pat->pis[1][i] == '1')?vdd_:0;

		stringstream ss;
		ss << "V" << netName << " " << netName << " 0 PWL(0ns " << v1 << " " << tranStartTime << "ns " << v1 << " " << tranEndTime << "ns " << v2 << " 5ns " << v2 << ")";
		sp.add( ss.str() );
	}

	// setup measurement
	setMeasurement(&sp);

	while(std::getline(fin,line))
	{
		if(line == ".END")
			setMeasurement(&sp);
		sp.add( line );
	}
	if(type == HSPICE)
		sp.spiceSim();
	else
		sp.nanoSim();
	
	fin.close();
	cout << "end" << endl;
	return true;
}

bool SpiceSimulator::extractResult(SimType type , string workspace , int no)
{
	pathDelay_.clear();
	extraPathDelay_.clear();
	stringstream ss;
	ss << no;
	string spFileName = workspace +"/pat"+ss.str()+"/pat.sp";
	ifstream fin;
	string line;
	if(type == HSPICE)
	{
		string result = spFileName + ".lis";

		fin.open(result.c_str());
		if(!fin)
		{
			cout << "**WARRN: SpiceSimulator::sim: can't open " << result << endl;
			cout << "**       Extraction fail!" << endl;
			return false;
		}
		cout << "open " << result << endl;

		bool firstPartDelay = false;
		bool secondPart = false;
		while(getline(fin , line))
		{
			bool findDelay = line.find("delay")!=string::npos;

			stringstream ss(line);
			if( findDelay && ! firstPartDelay)
				firstPartDelay = true;
			if( !findDelay && firstPartDelay)
				secondPart = true;
			
			if(line.find("fail")!=string::npos)
				continue;
				
			if(findDelay && firstPartDelay && ! secondPart)
			{
				//u1073_delay=  1.5075e-10    targ=   8.050e-10s  trig=   6.543e-10s
				string name;
				string in;
				double val1;
				double val2;
				ss >> name >> in >> in >> val1 >> in >> in >> val2 >> in;
				pathDelay_[name] = ((val1 + val2)/2);
				//cout << "1 " << name << " " << (val1 + val2)/2 << endl;
			}

			if(findDelay && secondPart)
			{
				//u1073_delay=  1.5075e-10    targ=   8.050e-10s  trig=   6.543e-10s
				string name;
				string in;
				double val1;
				double val2;
				ss >> name >> in >> in >> val1 >> in >> in >> val2 >> in;
				double delay = (val1 + val2)/2;
				extraPathDelay_[name] = (delay - pathDelay_[name]);
				pathDelay_[name] = ((val1 + val2)/2);
				//cout << "2 " << name << " " << (val1 + val2)/2 << endl;
			}

		}

	}
	else//nano-sim
	{
		// open the no-ir measurement
		string result = spFileName + "_output.meas";
		fin.open(result.c_str());
		if(!fin)
		{
			cout << "SpiceSimulator::sim: can't open " << result << endl;
			return false;
		}
		string in;
		
		// get all the delay and save into pathDelay_ 
		while(getline(fin , line))
		{
			if(line.find("fail")!=string::npos)
				continue;

			bool findDelay = line.find("delay")!=string::npos;

			stringstream ss(line);
			if(findDelay)
			{
				// measure name
				string name;

				// measure value
				double val1,val2;

				// get measurement
				// u1328_delay     =     9.609890E-11 TARG   =     1.055385E-09 TRIG  =     9.592857E-10
				ss >>  name >>     in >> in        >> in >>  in >> val1 >>      in >> in >> val2;

				// save measurement
				pathDelay_[name] = ((val1 + val2)/2);
				cout << "1 " << name << " " << (val1 + val2)/2 << endl;
			}
		}

		fin.close();
		// open the ir measuremnet
		result = spFileName + "_output.meas1";
		fin.open(result.c_str());
		if(!fin)
		{
			cout << "SpiceSimulator::sim: can't open " << result << endl;
			return false;
		}
		assert(fin);
		
		// get all the delay and save into pathDelay_ 
		while(getline(fin , line))
		{
			if(line.find("fail")!=string::npos)
				continue;

			bool findDelay = line.find("delay")!=string::npos;

			stringstream ss(line);
			if(findDelay)
			{
				// measure name
				string name;

				// measure value
				double val1,val2;

				// get measurement
				ss >>  name >>     in >> in        >> in >>  in >> val1 >>      in >> in >> val2;
				cout << name << endl;

				// save measurement
				extraPathDelay_[name] = ((val1 + val2)/2 - pathDelay_[name]);
				pathDelay_[name] = ((val1 + val2)/2);
				cout << "2 " << name << " " << (val1 + val2)/2 << endl;
			}
		}

	}
	return true;
}

void SpiceSimulator::setMeasurement(SpMaker *sp)
{
	
	for(unsigned i = 0 ; i < cir_->ff.size() ; ++i)
	{
		int cellID = cir_->ff[i];
		string cellName = cir_->cells[cellID].name;

		double averageV = vdd_ / 2;

		
		string nodeOptPin = cellName + "_D";
		
		bool rise = true;
		sp->setMeasure(nodeOptPin , averageV - 0.01 , rise , 
				   nodeOptPin , averageV + 0.01 , rise , cellName + "_R_delay");

		rise = false;
		sp->setMeasure(nodeOptPin , averageV + 0.01 , rise , 
				   nodeOptPin , averageV - 0.01 , rise , cellName + "_F_delay");
	}
	for(unsigned i = 0 ; i < cir_->po.size() ; ++i)
	{
		int netID = cir_->po[i];

		string cellName = cir_->nets[netID].name;

		double averageV = vdd_ / 2;

		string nodeOptPin = cellName;
		bool rise = true;
		sp->setMeasure(nodeOptPin , averageV - 0.01 , rise , 
				   nodeOptPin , averageV + 0.01 , rise , cellName + "_R_delay");

		rise = false;
		sp->setMeasure(nodeOptPin , averageV - 0.01 , rise , 
				   nodeOptPin , averageV + 0.01 , rise , cellName + "_F_delay");
	}
}

void SpiceSimulator::save(const string &fileName)
{
	ofstream fout(fileName.c_str());

	string maxPath;
	double maxPathDelay(0);
	
	for(map<string,double>::iterator it = pathDelay_.begin() ; it != pathDelay_.end() ; it++)
	{
		if(maxPathDelay < it->second)
		{
			maxPath = it->first;
			maxPathDelay = it->second;
		}
	}

	if(pathDelay_.size() == 0)
		fout << "maxPathDelay " << 0 << " "<< 0 << " " << 0 << endl;
	else
		fout << "maxPathDelay " << maxPath << " " <<  pathDelay_[maxPath]<< " " << extraPathDelay_[maxPath] << endl;
	
	fout << "nodeSize " << pathDelay_.size()<<endl;

	for(map<string,double>::iterator it = pathDelay_.begin() ; it != pathDelay_.end() ; it++)
		fout << it->first << " " << it->second << " " << extraPathDelay_[it->first] << endl;

	return;
}
