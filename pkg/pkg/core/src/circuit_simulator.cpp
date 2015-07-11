#include "circuit_simulator.h"
#include "delay_info.h"
#include "circuit.h"
#include "library.h"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>

//#define USEDEBUG
#ifdef USEDEBUG
#define DBG(x) std::cout << x
#define DBGF(x) x
#else
#define DBG(x) 
#define DBGF(x)
#endif

using namespace std;
CircuitSimulator::CircuitSimulator()
{
	cir = NULL;
	delayInfo = NULL;
	lib = NULL;
	cycleTime = -1;
}

CircuitSimulator::CircuitSimulator(const CircuitSimulator &sc)
{
	(*this) = sc;
	netWave = new Wave[cir->nets.size()];
}

CircuitSimulator::~CircuitSimulator()
{
	delete [] netWave;
}

bool CircuitSimulator::set(Circuit *circuit)
{
	cir = circuit;
	netWave = new Wave[cir->nets.size()];
	return true;
}

bool CircuitSimulator::set(Library *lib)
{
	this->lib = lib;
	return true;
}

bool CircuitSimulator::set(const string &clkName , Wave *clkWave , double clkTime)
{
	int netID = cir->getNetID(clkName);
	this->clkWave = *clkWave;
	cycleTime = clkTime;
	clkID = netID;
	return true;
}

bool CircuitSimulator::set(DelayInfo *delayInfo)
{
	this->delayInfo = delayInfo;
	return true;
}

bool CircuitSimulator::set(vector<int> *piOrder , vector<int> *poOrder, vector<int> *ppiOrder)
{
	this->piOrder = *piOrder;
	this->ppiOrder = *ppiOrder;
	this->poOrder = *poOrder;
	return true;
}


bool CircuitSimulator::initial(string &pi , string &ppi)
{
	if(piOrder.size() == 0)
	{
		cout << "CircuitSimulator::initial:please set the piOrder, poOrder and ppiOrder first!" <<endl;
		return false;
	}
	for(unsigned i = 0 ; i < pi.size() ; ++i)
	{
		int netID = piOrder[i];
		DBG( "NET: " << cir->nets[netID].name << "\t");
		if(netID == clkID)
			continue;
		switch(pi[i])
		{
			case 'X':
				netWave[netID].initialValue = Wave::X;
				break;
			case '1':
				netWave[netID].initialValue = Wave::H;
				break;
			case '0':
				netWave[netID].initialValue = Wave::L;
				break;
		}
		DBG( netWave[netID].initialValue <<endl);
	}

	for(unsigned i = 0 ; i < ppi.size() ; i++)
	{
		int cellID = ppiOrder[i];
		Wave::Value value = Wave::X;
		switch(ppi[i])
		{
			case 'X':
				value = Wave::X;
				break;
			case '1':
				value = Wave::H;
				break;
			case '0':
				value = Wave::L;
				break;
		}

		// setting net Q to value
		int pinQ = lib->getFanOutIndex(cir->cells[cellID].type , cir->dff_q);
		int netQ = cir->cells[cellID].opt_net_id[pinQ];
		if(netQ != -1)
			netWave[netQ].initialValue = value;
		
		// setting net QN to !value
		int pinQN = lib->getFanOutIndex(cir->cells[cellID].type , cir->dff_qn);
		int netQN = cir->cells[cellID].opt_net_id[pinQN];
		if(netQN != -1)
			netWave[netQN].initialValue = Wave::invert(value);
	}
	for(unsigned i = 0 ; i < cir->cells.size() ; i++)
		justifyInitialValue(&cir->cells[i]);
	return true;
}

bool CircuitSimulator::initial(int netID , bool value)
{
	netWave[netID].initialValue = (value == true)?Wave::H:Wave::L;
	return true;
}

bool CircuitSimulator::simulate(std::string &pi , double changeTime , bool capture)
{
	if(cir == NULL || delayInfo == NULL || lib == NULL || cir->dff_d == "")
	{
		cout << "CircuitSimulator::simulate: please set the circuit, delayInfo, lib, and DFF names" <<endl;
		return false;
	}

	for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
			netWave[i].size = 0;
	
	for(unsigned i = 0 ; i < pi.size() ; ++i)
	{
		int piNetID = piOrder[i];
		int val = Wave::X;
		switch(pi[i])
		{
			case '1':
				val = Wave::H;
				break;
			case '0':
				val = Wave::L;
				break;
		}
		if(netWave[piNetID].initialValue!= val){
			netWave[piNetID].addTransition(val , changeTime, 0.02, NULL, 0);
		}
	}
	
	netWave[clkID].size = 0;
	if(capture)
		netWave[clkID] = clkWave;
	else
		netWave[clkID].initialValue = Wave::L;

	DBG("clock net wave size : " << netWave[clkID].size << endl);
	DBGF(cout << "CLOCK" << endl;);
	DBGF(netWave[clkID].print());
	for(unsigned cellID = 0 ;cellID < cir->cells.size() ; ++cellID)
	{
		Cell &cell = cir->cells[cellID];
		DBG( "CELL=====================" << cell.name <<endl);
		if(cell.logic_type == DFF)
		{
			Wave *d = NULL; 
			Wave *si = NULL;
			Wave *se = NULL;
			Wave *q = NULL;
			Wave *qn = NULL;
			Wave *ck = NULL;
			DBG( "DFF---------" <<endl);

			int pinD = lib->getFanInIndex(cell.type , cir->dff_d);
			int pinQ = lib->getFanOutIndex(cell.type , cir->dff_q);
			int pinQN = lib->getFanOutIndex(cell.type , cir->dff_qn);
			int pinSI = lib->getFanInIndex(cell.type , cir->dff_si);
			int pinSE = lib->getFanInIndex(cell.type , cir->dff_se);
			int pinCK = lib->getFanInIndex(cell.type , cir->dff_ck);
			assert(pinD != -1);
			assert(pinSI != -1);
			assert(pinSE != -1);
			assert(pinCK != -1);
			int netD = cell.ipt_net_id[pinD];
			int netQ = cell.opt_net_id[pinQ];
			int netQN= cell.opt_net_id[pinQN];
			int netSI= cell.ipt_net_id[pinSI];
			int netSE= cell.ipt_net_id[pinSE];
			int netCK= cell.ipt_net_id[pinCK];

			if(netD != -1)
				d = &netWave[netD];
			if(netQ != -1)
				q = &netWave[netQ];
			if(netQN != -1)
				qn = &netWave[netQN];
			if(netSI != -1)
				si = &netWave[netSI];
			if(netSE != -1)
				se = &netWave[netSE];
			if(netCK != -1)
				ck = &netWave[netCK];

			if(ck->size == 0){
				DBG("clk not trigger!" << endl);
				continue;
			
			}
			//cout << "here?" << endl;
			Wave *target = NULL;
			if(si == NULL)
				target = d;
			else
				target = (se->initialValue == Wave::L)? d : si;
			//cout << "check?" << endl;

			if(q != NULL && q->initialValue != target->initialValue){
				
				q->addTransition(target->initialValue , ck->transition[0].time, ck->transition[0].period, &ck->transition[0], 0);
				
				double riseDelayQ = delayInfo->getGateDelay(cellID , pinQ , pinCK , true);
				double fallDelayQ = delayInfo->getGateDelay(cellID , pinQ , pinCK , false);
				q->addDelay(riseDelayQ , fallDelayQ);
				
				int pinIndex = pinQ;
				int relativePinIndex = pinCK;
				q->addPeriod(cell.type , relativePinIndex , pinIndex , cir->nets[netQ].totalRiseCap , cir->nets[netQ].totalFallCap , lib);
			}
			//cout << "hello" << endl;
			if(qn != NULL && qn->initialValue == target->initialValue)
			{
				qn->addTransition(Wave::invert(target->initialValue) , ck->transition[0].time, ck->transition[0].period, &ck->transition[0],0);
				
				double riseDelayQN = delayInfo->getGateDelay(cellID , pinQN , pinCK , true);
				double fallDelayQN = delayInfo->getGateDelay(cellID , pinQN , pinCK , false);
				qn->addDelay(riseDelayQN , fallDelayQN);

				int pinIndex = pinQN;
				int relativePinIndex = pinCK;
				qn->addPeriod(cell.type , relativePinIndex , pinIndex , cir->nets[netQ].totalRiseCap , cir->nets[netQ].totalFallCap , lib);
			}
			//cout << "nono" << endl;
			if(q != NULL){
				DBG( "Q" <<endl);
				DBGF(q->print());
			}
			if(qn != NULL){
				DBG( "QN" <<endl);
				DBGF(qn->print());
			}
			//cout << "yaya" << endl;
			continue;
		}
		//cout << "noway" << endl;
		DBGF(
		switch(cell.logic_type)
		{
			case AND: 
				DBG( "AND GATE-------" <<endl);
				break;
			case OR:
				DBG( "OR GATE-------" <<endl);
				break;
			case NOR:
				DBG( "NOR GATE-------" <<endl);
				break;
				break;
			case NAND:
				break;
			case INV:
				DBG( "INV GATE-------" <<endl);
				break;
			case BUF:
				DBG( "BUF GATE-------" <<endl);
				break;
			case LOGIC1:
				DBG( "LOGIC1 GATE-------" <<endl);
				break;
			case LOGIC0:
				DBG( "LOGIC0 GATE-------" <<endl);
				break;
			case XOR:
				DBG( "XOR GATE-------");
				break;
			case XNOR:
				DBG( "XNOR GATE-------");
				break;
			case DFF:
				assert(0);
		});
		//cout << "kill people" << endl;
		Wave optWave;
		if(cell.logic_type == LOGIC1)
			optWave.initialValue = Wave::H;
		else if(cell.logic_type == LOGIC0)
			optWave.initialValue = Wave::L;
		//cout << "fuck you" << endl;
		int optNet = cell.opt_net_id[0];
		for(unsigned ipt = 0; ipt < cell.ipt_net_id.size() ; ++ipt)
		{
			int optPin = 0;
			int iptPin = ipt;
			//cout << ipt << endl;

			if(cell.ipt_net_id[ipt] == -1)
				continue;


			// cell delay
			double cellRiseDelay = delayInfo->getGateDelay(cellID , optPin , iptPin , true);
			double cellFallDelay = delayInfo->getGateDelay(cellID , optPin , iptPin , false);
			
			// ipt wave calculate
			Wave temp;
			temp.copyAndLinkTransition(netWave[cell.ipt_net_id[ipt]]);
			DBGF(cout << "INPUT " << endl;temp.print(););
			temp.addDelay(cellRiseDelay, cellFallDelay);
			temp.addPeriod(cell.type , iptPin , optPin , cir->nets[optNet].totalRiseCap , cir->nets[optNet].totalFallCap , lib);
			
			if(ipt == 0){
				// wave initial
				//optWave = temp;
				memcpy( &optWave, &temp, sizeof(Wave) );
				continue;
			}
			else
				// wave overlap
				switch(cell.logic_type)
				{
					case AND: 
					case NAND:
						optWave._and(temp);
						break;
					case OR:
					case NOR:
						optWave._or(temp);
						break;
					case XOR:
					case XNOR:
						optWave._xor(temp);
						break;
					case INV:
					case BUF:
					case LOGIC1:
					case LOGIC0:
					case DFF:
						assert(0);
						break;
				}
		}
		//cout << "ffjfj" << endl;
		switch(cell.logic_type)
		{
			case NAND:
			case NOR:
			case INV:
			case XNOR:
				optWave.inv();
				break;
			default:
				break;
		}
		//cout << "or here" << endl;
		int optID = cell.opt_net_id[0];
		swap(netWave[optID] , optWave);
		DBGF(netWave[optID].print());
	}
	return true;
}


bool CircuitSimulator::checkTiming()
{
	return true;
}

void CircuitSimulator::justifyInitialValue(Cell *cell)
{
	if(cell->logic_type == DFF)
		return;
	DBGF(
		switch(cell->logic_type)
		{
			case AND:
				cout << "AND" <<endl;
				break;
			case NAND:
				cout << "NAND" << endl;
				break;
			case OR:
				cout << "OR" << endl;
				break;
			case NOR:
				cout << "NOR" <<endl;
				break;
			case INV:
				cout << "INV" <<endl;
				break;
			case BUF:
				cout << "BUF" <<endl;
				break;
			case DFF:
				cout << "DFF" <<endl;
				break;
			case XOR:
				cout << "XOR" <<endl;
				break;
			case XNOR:
				cout << "XNOR" << endl;
				break;
			case LOGIC1:
				cout << "LOGIC1" <<endl;
				break;
			case LOGIC0:
				cout << "LOGIC0" <<endl;
				break;
		}
	);
	
	if(cell->logic_type == LOGIC1)
	{
		netWave[cell->opt_net_id[0]].initialValue = Wave::H;
		return;
	}
	else if(cell->logic_type == LOGIC0)
	{
		netWave[cell->opt_net_id[0]].initialValue = Wave::L;
		return;
	}
	DBG( "cell: " << cell->name <<endl);
	int val = netWave[cell->ipt_net_id[0]].initialValue;
	bool odd = false;
	for(unsigned i = 0 ; i < cell->ipt_net_id.size() ; ++i)
	{
		int netID = (cell->ipt_net_id[i]);
		if(netID == -1)
			continue;
		int netVal = netWave[netID].initialValue;
		DBG( "input: " << cir->nets[netID].name << " " << netVal <<endl);
		switch(cell->logic_type)
		{
			case AND: 
			case NAND:
				val &= netVal;
				break;
			case OR:
			case NOR:
				val |=netVal;
				break;
			case INV:
				val = Wave::invert(netVal);
				break;
			case BUF:
				val = netVal;
				break;
			case LOGIC1:
				val = Wave::H;
				break;
			case LOGIC0:
				val = Wave::L;
				break;
			case XOR:
			case XNOR:
				if(netVal == Wave::H)
					odd = !odd;
				break;
			case DFF:
				return;
		}
	}
	if(cell->logic_type == NAND or cell->logic_type == NOR)
		val = Wave::invert(val);
	if(cell->logic_type == XOR)
		val = (odd)?Wave::H:Wave::L;
	if(cell->logic_type == XNOR)
		val = (odd)?Wave::L:Wave::H;

	netWave[cell->opt_net_id[0]].initialValue = val;
	DBGF
	(
		cout << cir->nets[cell->opt_net_id[0]].name << "\t" ;
		cout << netWave[cell->opt_net_id[0]].initialValue <<endl;
		cout << "-------------------------" <<endl;
	);
	
}


bool CircuitSimulator::checkValue(std::string &po , std::string &ppo , bool capture)
{
	for(unsigned i = 0 ; i < po.size() ; ++i)
	{
		int netID = poOrder[i];
		Wave &wave = netWave[netID];
		Wave::Value value = Wave::X;
		switch(po[i])
		{
			case 'X':
				value = Wave::X;
				break;
			case '1':
				value = Wave::H;
				break;
			case '0':
				value = Wave::L;
				break;
		}
		int ci_value = (wave.size == 0)?wave.initialValue:wave.transition[wave.size-1].value;
		DBG( "check net " << cir->nets[netID].name << endl );
		if(ci_value != value)
		{
			
			return false;
		}
	}
	for(unsigned i = 0 ; i < ppo.size() ; i++)
	{
		int cellID = ppiOrder[i];
		Wave::Value value = Wave::X;
		switch(ppo[i])
		{
			case 'X':
				value = Wave::X;
				break;
			case '1':
				value = Wave::H;
				break;
			case '0':
				value = Wave::L;
				break;
		}
		if(capture)
		{
			int pinD = lib->getFanInIndex(cir->cells[cellID].type, cir->dff_d);
			int netD = cir->cells[cellID].ipt_net_id[pinD];
			Wave &wave = netWave[netD];
			int sim_value = (wave.size ==0)?wave.initialValue:wave.transition[wave.size-1].value;
			DBG("check net " << cir->nets[netD].name << endl );
			if(sim_value != value)
				return false;
		}
		else
		{
			int pinQ = lib->getFanOutIndex(cir->cells[cellID].type, cir->dff_q);
			int netQ = cir->cells[cellID].opt_net_id[pinQ];
			assert(netQ != -1);
			Wave &wave = netWave[netQ];
			DBG("check net " << cir->nets[netQ].name << endl );
			int sim_value = (wave.size == 0)?wave.initialValue:wave.transition[wave.size-1].value;
			if(sim_value != value)
				return false;
		}
	}
	return true;
}

std::string CircuitSimulator::getPPO()
{
	string ret;
	for(unsigned i = 0 ; i < ppiOrder.size() ; i++)
	{
		int cellID = ppiOrder[i];
		int pinD = lib->getFanInIndex(cir->cells[cellID].type, cir->dff_d);
		int netD = cir->cells[cellID].ipt_net_id[pinD];
		assert(netD != -1);
		Wave &wave = netWave[netD];
		int sim_value = (wave.size == 0)?wave.initialValue:wave.transition[wave.size-1].value;
		int value = -1;
		switch(sim_value)
		{
			case Wave::X:
				value = 'X';
				break;
			case Wave::H:
				value = '1';
				break;
			case Wave::L:
				value = '0';
				break;
		}
		ret.push_back(value);
	}
	return ret;
	
}
