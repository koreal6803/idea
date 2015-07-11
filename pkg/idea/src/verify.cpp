#include "verify.h"
#include "core/src/circuit.h"
#include "idea.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <sys/stat.h>
using namespace std;


void VerifyIdea::set(Library *lib)
{
	this->lib = lib;
}

void VerifyIdea::set(Idea *idea)
{
	this->idea = idea;
}
void VerifyIdea::pathVerify(Transition *t , double pathDelay , double extraDelay)
{
	double originPathDelayIdea = t->time;
	double extraPathDelayIdea(0);
	const Transition *h = t;
	while(h)
	{
		///* look into path data information

		// get the net of transition
		int netID = t->netID;
		
		// get the net of previous transition
		int prevNetID = -1;
		if(h->prevTransition)
			prevNetID = t->prevTransition->netID;
		else
			break;// there are no previous transition anymore

		// get the cell between above two transition
		int cellID = cir_->nets[netID].ipt_cell_id;
		string cellName = cir_->cells[cellID].name;

		cout << cir_->nets[netID].name << "\t" << cellName << "\t" << h->time <<" " << cir_->nets[netID].totalRiseCap << " " << h->value<< endl;

		int transitionID = idea->transitionIDs[t];
		int center = idea->extraGateDelays.size()/2;

		extraPathDelayIdea += idea->extraGateDelays[center][transitionID];
		h = h->prevTransition;
	}
	spicePathDelay.push_back(pathDelay*1e9);
	spicePathDelayIR.push_back((pathDelay+extraDelay)*1e9);
	ideaPathDelay.push_back(originPathDelayIdea);
	ideaPathDelayIR.push_back(originPathDelayIdea+extraPathDelayIdea);
	return;
}



bool VerifyIdea::verify(Circuit &cir, Wave *waves, int patternID , const string &workspace)
{
	cir_ = &cir;
	// circuit directory
	stringstream ss;
	ss << patternID;
	string patDir = workspace + "/pat" + ss.str();

	// open hspice simulation result
	string simResult = patDir + "/sp_result.txt";
	ifstream fin(simResult.c_str());

	// if hspice result doesn't exist, try nanosim simulation result
	if(!fin)
	{
		cout << "**WARRN: can't find result in " << patDir << "/sp_result.txt" << endl;
		return false;
		string simResult2 = patDir + "/nano_result.txt";
		fin.open(simResult2.c_str());
	}

	// if both result not exist, stop verify
	if(!fin)
	{
		cout << "**WARRN: can't find result in " << patDir << "/nano_result.txt" << endl;
		return false;
	}

	string measName;
	double pathDelay;
	double extraDelay;

	fin >> measName >> measName >> pathDelay >> extraDelay;

	// there is no transition in po or ppo
	// happend at s27 pattern 1
	if(pathDelay == 0 && extraDelay == 0)
		return true;
	
	// measure names in sp_result have an addition character '='
	// we need to delete it
	if(*measName.rbegin() == '=')
		measName.erase(measName.end()-1);
	
	// remove _r_delay or _f_delay
	measName.erase(measName.end()-8,measName.end());
	cout << "test: " << measName << endl;


	
	int cellID = cir.getCellID(measName);
	int netID = -1;
	if(cellID == -1)
		netID = cir.getNetID(measName);
	
	if(cellID == -1 && netID == -1)
	{
		cout << "can't match po or ppo" << endl;
		return false;
	}

	
	if(netID == -1)
		netID = cir.cells[cellID].ipt_net_id[0];
	

	cout << "net name: " << cir.nets[netID].name << endl;
	
	if(waves[netID].transition.size() == 0)
		return false;

	const Transition *lastTrans = &waves[netID].transition.back();
		
	pathVerify(lastTrans , pathDelay , extraDelay);
	
	cout << "success" << endl;
	return true;
}
void VerifyIdea::singleVerify(Circuit &cir, Wave *waves, pgNs::PowerGrid &pg, const Transition *trans, VERIFY_TYPE type)
{
	std::string fileName = "../../spCombine/circuit/" + cir.getName() + ".sp";
	std::ifstream fin(fileName.c_str());
	if(!fin)
	{
		std::cout << "**ERROR VerifyIdea::verify: Can't find file " << fileName << std::endl;
		return;
	}
	string spFileName = "./verify/test.sp";
	SpMaker sp(spFileName);
	std::string line;
	while(std::getline(fin , line))
	{
		//cout << line << endl;
		sp.add( line );
		if(line == "* INPUT")
			break;
	}
/*
V1 G0 0 PWL(0ns 0v 2ns 0v 2.001ns 1.1v 4ns 1.1v)
V2 G1 0 PWL(0ns 0v 2ns 0v 2.001ns 1.1v 4ns 1.1v)
V3 G2 0 PWL(0ns 1.1v 2ns 1.1v 2.001ns 0v 4ns 0v)
V4 G3 0 PWL(0ns 1.1v 2ns 1.1v 2.001ns 0v 4ns 0v)
V5 test_se 0 0v
V6 test_si 0 1.1v*/
	
	// setup voltage source
	for(unsigned i = 0 ; i < pg.vddNodes.size() ; ++i){
		string nodeName(pg.getNodes()[pg.vddNodes[i]].getName());
		size_t found = nodeName.find(".");
		assert(found != string::npos);
		nodeName[found] = '_';
		sp.add("VDDSOURCE " + nodeName + " 0 1.1v");
	}
	for(unsigned i = 0 ; i < pg.gndNodes.size() ; ++i){
		string nodeName(pg.getNodes()[pg.gndNodes[i]].getName());
		size_t found = nodeName.find(".");
		assert(found != string::npos);
		nodeName[found] = '_';
		sp.add("VSSSOURCE " + nodeName + " 0 0v");
	}

	// setup PPI
	const vector<Cell> &cells = cir.getCells();
	const vector<Net> &nets = cir.getNets();
	string ffstring = "* flip-flop switch";
	sp.add( ffstring );
	int qPin = lib->getFanoutIndex(cells[cir.ff[0]].type, "Q");
	int qnPin = lib->getFanoutIndex(cells[cir.ff[0]].type, "QN");
	for(unsigned i = 0 ; i < cir.ff.size() ; ++i)
	{
		int cellID = cir.ff[i];
		// print Q
		int netQ = cells[cellID].opt_net_id[qPin];
		if(netQ != -1){
			// string line = printNet(cells[cellID].name + "_Q" ,waves[netQ]);
			string qNode = cells[cellID].name + "_Q";
			double val = (waves[netQ].initialValue == Wave::H)?pg.getSupplyVoltage():0;
			stringstream ss;
			ss << val;
			sp.add(".ic v("+qNode+")="+ss.str());
			if(val != 0){
				sp.add(".ic v(X"+cells[cellID].name+"."+"N_8_M8_s"+")=1.1v");
				sp.add(".ic v(X"+cells[cellID].name+"."+"n_5_m20_g"+")=1.1v");
			}else
			{
				sp.add(".ic v(X"+cells[cellID].name+"."+"N_8_M8_s"+")=0v");
				sp.add(".ic v(X"+cells[cellID].name+"."+"n_5_m20_g"+")=0v");
			}
		}

		// print QN
		int netQN = cells[cellID].opt_net_id[qnPin];
		if(netQN != -1){
			//string line = printNet(cells[cellID].name + "_QN",waves[netQN]) ;
			//sp.add( line);
			string qNode = cells[cellID].name + "_QN";
			double val = (waves[netQN].initialValue == Wave::H)?pg.getSupplyVoltage():0;
			stringstream ss;
			ss << val;
			sp.add(".ic v("+qNode+")="+ss.str());
		}
	}
	sp.add("* PI switch");
	// setup PI
	for(unsigned i = 0 ; i < cir.pi.size() ; ++i)
	{
		int netID = cir.pi[i];
		string line =  printNet(nets[netID].name,waves[netID]);
		sp.add( line );
	}
	if(trans == NULL)
	{
		spicePathDelay.push_back(0);
		spicePathDelayIR.push_back(0);
		ideaPathDelay.push_back(0);
		ideaPathDelayIR.push_back(0);
	}

	// setup measurement
	setPathMeasurement(true, sp, trans, cir);

	while(std::getline(fin,line))
	{
		if(line == ".END")
			setPathMeasurement(false, sp, trans, cir);
		sp.add( line );
	}
	if(type == HSPICE)
		sp.spiceSim();
	else
		sp.nanoSim();
	printResult(sp, trans, cir, waves);
}
void VerifyIdea::setPathMeasurement(bool ideal, SpMaker &sp , const Transition *trans, Circuit &cir)
{
	sp.add("* Measurement");
	const vector<Cell> &cells = cir.getCells();
	const vector<Net> &nets = cir.getNets();
	const Transition *t = trans;
	while(t)
	{
		// get the net of transition
		int netID = t->netID;

		// get the net of previous transition
		int prevNetID = -1;
		if(t->prevTransition)
			prevNetID = t->prevTransition->netID;
		else
			break;// there are no previous transition anymore

		// get the cell between above two transition
		int cellID = nets[netID].ipt_cell_id;
		string cellName = cells[cellID].name;
		
		// get pin name connected the cell and net
		vector<int>::const_iterator optPinIDIter;// = find(cells[cellID].opt_net_id.begin() , cells[cellID].opt_net_id.end() , netID);
		for(optPinIDIter = cells[cellID].opt_net_id.begin() ; optPinIDIter != cells[cellID].opt_net_id.end() ; optPinIDIter++)
			if(*optPinIDIter == netID)
				break;
		assert(optPinIDIter != cells[cellID].opt_net_id.end());
		int optPinID = optPinIDIter - cells[cellID].opt_net_id.begin();
		string optPinName = lib->getFanoutName(cells[cellID].type , optPinID);
		
		// get pin name connected the cell and previous net
		vector<int>::const_iterator iptPinIDIter;// = find(cells[cellID].ipt_net_id.begin() , cells[cellID].ipt_net_id.end() , prevNetID);
		for(iptPinIDIter = cells[cellID].ipt_net_id.begin() ; iptPinIDIter != cells[cellID].ipt_net_id.end() ; iptPinIDIter++)
			if(*iptPinIDIter == prevNetID)
				break;
		assert(iptPinIDIter != cells[cellID].ipt_net_id.end());
		int iptPinID = iptPinIDIter - cells[cellID].ipt_net_id.begin();
		string iptPinName = lib->getFaninName(cells[cellID].type , iptPinID);

		string nodeOptPin = cellName + "_" + optPinName;
		string nodeIptPin = cellName + "_" + iptPinName;
		bool optPinRise = t->value == Wave::H;
		bool iptPinRise = t->prevTransition->value == Wave::H;
		double averageV = 0.55;
		
		sp.setMeasure(nodeIptPin , averageV , iptPinRise , 
				   nodeOptPin , averageV , optPinRise , cellName + "_delay");
		double vstart = (iptPinRise)?0.33:0.77;
		double vend = (iptPinRise)?0.77:0.33;
		sp.setMeasure(nodeIptPin , vstart , iptPinRise , 
				   nodeIptPin , vend , iptPinRise , cellName + "_transition");
		vstart = (optPinRise)?0.33:0.77;
		vend = (optPinRise)?0.77:0.33;
		//.meas TRAN U11_transition TRIG v(U11_A1) VAL=0.77 fall=1  TARG v(U11_A1) VAL=0.33 fall=1 
		//os << ".meas TRAN " << cellName << "_vh TRIG v(" << nodeOptPin << ") VAL=" << vstart << " fall=" << optPinRise << " TRAG v(" << nodeOptPin << ") VAL=" << vend << " fall=" << optPinRise;
		//sp.add(os.str());
		//sp.setMeasure(nodeOptPin , vstart , optPinRise , nodeOptPin , vend , optPinRise , cellName + "_vh");
		t = t->prevTransition;
	}
	/*void setMeasure(const std::string &node1 ,double value1, bool rise1, 
							 const std::string &node2, double value2, bool rise2 , const std::string& measureName);*/
}

void VerifyIdea::printResult(SpMaker &sp , const Transition *trans, Circuit &cir, Wave *wave)
{
	const vector<Cell> &cells = cir.getCells();
	const vector<Net> &nets = cir.getNets();
	const Transition *t = trans;
	cout << "=========================Critical Path=============================" << endl;
	cout << endl;
	double pathDelaySpiceIR(0);
	double extraPathDelayIdea(0);
	double pathDelaySpice(0);
	double pathDelayIdea(0);
	bool lastGate = true;
	while(t)
	{
		// get the net of transition
		int netID = t->netID;

		// get the net of previous transition
		int prevNetID = -1;
		if(t->prevTransition)
			prevNetID = t->prevTransition->netID;
		else
			break;// there are no previous transition anymore

		// get the cell between above two transition
		int cellID = nets[netID].ipt_cell_id;
		string cellName = cells[cellID].name;

		double cap = (t->value==Wave::H)?nets[netID].totalRiseCap:nets[netID].totalFallCap;
		vector<Measurement >measure = sp.getAlterMeasure(cellName + "_delay");
		vector<Measurement >measuret = sp.getAlterMeasure(cellName + "_transition");
		double spiceDelay = measure[0].gateDelay*1e9;
		double spiceDelay2 = measure[1].gateDelay*1e9;
		double gateDelay = t->time - t->prevTransition->time;
		int transitionID = idea->transitionIDs[t];
		double extraGateDelay = idea->extraGateDelays[idea->extraGateDelays.size()/2][transitionID];
		double vl = idea->vls[transitionID];
		double vh = idea->vhs[transitionID];
		
		cout << "----------------------------------" << endl;
		cout << "cell name        : \t" << cellName << endl;
		cout << "cell type        : \t" << cir.cell_types[cells[cellID].type] << endl;
		cout << "value            : \t" << t->value << endl;
		cout << "time             : \t" << t->time << endl;
		cout << "spice delay      : \t" << measure[0].gateDelay*1e9 <<endl;
		cout << "spice extra delay: \t" << measure[1].gateDelay*1e9 - measure[0].gateDelay*1e9 <<endl;
		cout << "spice ir factor  : \t" << (measure[1].gateDelay*1e9 - measure[0].gateDelay*1e9)/measure[0].gateDelay*1e-9<<endl;
		cout << "library delay    : \t" << gateDelay << " " << gateDelay/spiceDelay << " " << gateDelay/spiceDelay2<< endl;
		cout << "idea extra delay : \t" << extraGateDelay << endl;
		cout << "idea ir factor   : \t" << extraGateDelay/gateDelay << endl;
		cout << "vdd, gnd         : \t" << vh << " " << vl << endl;
		cout << "real transition: : \t" << measuret[0].gateDelay*1e9 <<" " <<  measuret[1].gateDelay*1e9 << endl;
		cout << "idea ipt transition  : \t" << t->prevTransition->period << endl;
		cout << "idea opt cap     : \t" << cap << endl;
		
		if(lastGate){
			pathDelaySpiceIR = measure[1].endTime;
			pathDelaySpice = measure[0].endTime;
			pathDelayIdea = t->time;
			lastGate = false;
		}
		extraPathDelayIdea += extraGateDelay;
		t = t->prevTransition;
	}
	cout << "path delay spice   : " << pathDelaySpice << endl;
	cout << "path delay spice IR: " << pathDelaySpiceIR << endl;
	cout << "path delay idea    : " << pathDelayIdea << endl;
	cout << "path delay idea IR : " << pathDelayIdea + extraPathDelayIdea << endl;

	cout << "error      : " << (pathDelayIdea - pathDelaySpice*1e9)/pathDelaySpice<<endl;
	cout << "extra error: " << (extraPathDelayIdea - (pathDelaySpiceIR - pathDelaySpice)*1e9)/(pathDelaySpiceIR - pathDelaySpice)/1e9<<endl;
	cout << "portion    : " << (pathDelaySpiceIR - pathDelaySpice) / pathDelaySpiceIR<<endl;
}

std::string VerifyIdea::printNet(const std::string &nodeName, Wave &wave)
{
	std::string voltageSource = "V" + nodeName;
	std::string ret;
	if(wave.transition.empty()){
		if(wave.initialValue == Wave::H)
			ret = voltageSource + " " + nodeName + " 0 1.1v";
		else if(wave.initialValue == Wave::L)
			ret = voltageSource + " " + nodeName + " 0 0v";
	}
	else{
		double tranStartTime = wave.transition[0].time - wave.transition[0].period/2*5/2;
		double tranEndTime = wave.transition[0].time + wave.transition[0].period/2*5/2;
		double vdd = 1.1;
		double gnd = 0;
		stringstream ss;

		if(wave.initialValue == Wave::H)
			ss << voltageSource << " " << nodeName << " 0 PWL(0ns " << vdd << " " << tranStartTime << "ns " << vdd << " " << tranEndTime << "ns " << gnd << " 5ns " << gnd << ")";
		else if(wave.initialValue == Wave::L)
			ss << voltageSource << " " << nodeName << " 0 PWL(0ns " << gnd << " " << tranStartTime << "ns " << gnd << " " << tranEndTime << "ns " << vdd << " 5ns " << vdd << ")";
		ret = ss.str();
	}
	return ret;
}
