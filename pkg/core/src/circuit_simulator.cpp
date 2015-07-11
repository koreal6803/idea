#include "circuit_simulator.h"
#include "circuit.h"
#include "library.h"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstring>


#define USEDBUG

//#define USEDEBUG
#ifdef USEDEBUG
#define DBG(x) std::cout << x
//#define DBGF(x) x
#else
#define DBG(x) 
#define DBGF(x)
#endif

using namespace std;
CircuitSimulator::CircuitSimulator()
{
	cir = NULL;
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


bool CircuitSimulator::set(vector<int> *piOrder , vector<int> *poOrder, vector<int> *ppiOrder)
{
	this->piOrder = *piOrder;
	this->ppiOrder = *ppiOrder;
	this->poOrder = *poOrder;
	return true;
}


bool CircuitSimulator::initial(string &pi , string &ppi)
{
	// piOrder contain the ids of nets in sequence of test vector
	// ppiOrder contain the ids of flip flop in sequence of test vector
	if(piOrder.size() == 0 || ppiOrder.size() == 0)
	{
		cout << "CircuitSimulator::initial:please set the piOrder, poOrder and ppiOrder first!" <<endl;
		return false;
	}

	// assign pi to nets
	for(unsigned i = 0 ; i < pi.size() ; ++i)
	{
		int netID = piOrder[i];
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
	}
	
	// assign ppo to nets
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
		int pinQ = lib->getFanoutIndex(cir->cells[cellID].type , cir->dff_q);
		int netQ = cir->cells[cellID].opt_net_id[pinQ];
		if(netQ != -1){
			netWave[netQ].initialValue = value;
		}
		
		// setting net QN to !value
		int pinQN = lib->getFanoutIndex(cir->cells[cellID].type , cir->dff_qn);
		int netQN = cir->cells[cellID].opt_net_id[pinQN];
		if(netQN != -1)
			netWave[netQN].initialValue = Wave::invert(value);
	}

	// justify time frame 1
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
	if(cir == NULL || lib == NULL || cir->dff_d == "")
	{
		cout << "CircuitSimulator::simulate: please set the circuit, delayInfo, lib, and DFF names" <<endl;
		return false;
	}

	for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
			netWave[i].transition.clear();
	
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
			Transition piTransition;
			piTransition.value = val;
			piTransition.time = changeTime;
			piTransition.period = 0.01;
			piTransition.prevTransition = NULL;
			netWave[piNetID].addTransition(val , piTransition);
		}
	}
	
	if(capture)
		netWave[clkID] = clkWave;
	else
	{
		netWave[clkID].transition.clear();
		netWave[clkID].initialValue = Wave::L;
	}

	for(unsigned cellID = 0 ;cellID < cir->cells.size() ; ++cellID)
	{
		// get cell 
		Cell &cell = cir->cells[cellID];
		// DFF
		if(cell.logic_type == DFF)
		{
			// find net wave of this flip flop
			Wave *d = NULL; 
			Wave *si = NULL;
			Wave *se = NULL;
			Wave *q = NULL;
			Wave *qn = NULL;
			Wave *ck = NULL;
			
			// get pin ID of this flip flop
			int pinD = lib->getFaninIndex(cell.type , cir->dff_d);
			int pinQ = lib->getFanoutIndex(cell.type , cir->dff_q);
			int pinQN = lib->getFanoutIndex(cell.type , cir->dff_qn);
			int pinSI = lib->getFaninIndex(cell.type , cir->dff_si);
			int pinSE = lib->getFaninIndex(cell.type , cir->dff_se);
			int pinCK = lib->getFaninIndex(cell.type , cir->dff_ck);
			assert(pinD != -1);
			assert(pinSI != -1);
			assert(pinSE != -1);
			assert(pinCK != -1);

			// use pinID to find the netID associated with this flip flop
			int netD = cell.ipt_net_id[pinD];
			int netQ = cell.opt_net_id[pinQ];
			int netQN= cell.opt_net_id[pinQN];
			int netSI= cell.ipt_net_id[pinSI];
			int netSE= cell.ipt_net_id[pinSE];
			int netCK= cell.ipt_net_id[pinCK];
			
			// if there exist a net connect to the pin, record the net's wave
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
			
			// if the clock is not trigger, then the fanout value of this flip flop will not change
			if(ck->transition.size() == 0)
				continue;
			
			// now, we need to update the value of Q, which might be same value of pin SI or pin D.
			// we need to use SE to find out the value of Q is the value of SI or D

			// target will be wave of SI or D
			Wave *target = NULL;
			
			// if SI pin does't connect any net, then value of Q is depend on value of D
			if(si == NULL)
				target = d;
			// if SI and D both exist, then use SE to decide which value should propagate
			else
				target = (se->initialValue == Wave::L)? d : si;
			
			// after find out the value of next state, lets assign the value to Q and QN
			if(q != NULL && q->initialValue != target->initialValue){
				
				// when clk toggle, Q will have a transiiton, whose value will be target->initialValue
				// and the other information will copy from clk's first transition
				q->addTransition(target->initialValue , ck->transition[0]);
				// cout << "FF: " << cir->nets[netQ].name << " " << target->initialValue << endl;

				// the transiiton of Q is caused by clk
				q->transition.back().prevTransition = &ck->transition[0];
				
				// we are going to look up the library and add delay and tranisiton time to Q wave

				// before look up the value in library, we need to find out input pin and output pin of cell
				int pinIndex = pinQ;
				int relativePinIndex = pinCK;
				
				// use library to find delay and transition time
				q->addDelay(cell.type , relativePinIndex , pinIndex , cir->nets[netQ].totalRiseCap , cir->nets[netQ].totalFallCap , lib);
				q->addPeriod(cell.type , relativePinIndex , pinIndex , cir->nets[netQ].totalRiseCap , cir->nets[netQ].totalFallCap , lib);
			}
			
			// QN will be the same as Q
			if(qn != NULL && qn->initialValue == target->initialValue)
			{
				qn->addTransition(Wave::invert(target->initialValue) , ck->transition[0]);//.time, ck->transition[0].period, &ck->transition[0],0);
				qn->transition.back().prevTransition = &ck->transition[0];
				// cout << "FF: " << cir->nets[netQN].name << " " << Wave::invert(target->initialValue) << endl;
				
				int pinIndex = pinQN;
				int relativePinIndex = pinCK;

				qn->addDelay(cell.type , relativePinIndex , pinIndex , cir->nets[netQN].totalRiseCap , cir->nets[netQN].totalFallCap , lib);
				qn->addPeriod(cell.type , relativePinIndex , pinIndex , cir->nets[netQN].totalRiseCap , cir->nets[netQN].totalFallCap , lib);
			}
			continue;
		}

		// assumption: only single gate output
		int optPin = 0;
		int optNet = cell.opt_net_id[optPin];
		Wave &optWave = netWave[optNet];

		if(cell.logic_type == LOGIC1)
			optWave.initialValue = Wave::H;
		else if(cell.logic_type == LOGIC0)
			optWave.initialValue = Wave::L;

		for(unsigned ipt = 0; ipt < cell.ipt_net_id.size() ; ++ipt)
		{
			//cout << " " << cir->nets[cell.ipt_net_id[ipt]].name << endl;
			// there might have some pin without connection
			if(cell.ipt_net_id[ipt] == -1)
				continue;

			Wave &temp = netWave[cell.ipt_net_id[ipt]];
			
			// we want to merge the waves of input
			if(ipt == 0)
			{
				// copy the first wave
				optWave._buf(temp , ipt);
			}
			else
				// wave merging 
				switch(cell.logic_type)
				{
					case AND: 
					case NAND:
						optWave._and(temp , ipt);
						break;
					case OR:
					case NOR:
						optWave._or(temp , ipt);
						break;
					case XOR:
					case XNOR:
					case INV:
					case BUF:
					case LOGIC1:
					case LOGIC0:
					case DFF:
						assert(0);
						break;
				}
		}
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
		optWave.propagate(cell.type, 0, cir->nets[optNet].totalRiseCap, cir->nets[optNet].totalFallCap, lib);
		//cout << "CELL OPT:  ----------------------------------------------------------------- " << cell.name << endl;
		//optWave.print();
	}
	//for(unsigned i = 0 ; i < cir->nets.size() ; i++)
	//{
		//cout << "NET OPT:  ----------------------------------------------------------------- " << cir->nets[i].name << endl;
		//netWave[i].print();
	//}
	for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
		for(unsigned j = 0 ; j < netWave[i].transition.size() ; j++)
			netWave[i].transition[j].netID = (int)i;
	return true;
}

// justify the initial value of fanout of combinational logic
void CircuitSimulator::justifyInitialValue(Cell *cell)
{
	// skip filp flop
	if(cell->logic_type == DFF)
		return;
	
	// assnign logic to 1 to cell LOGIC1
	if(cell->logic_type == LOGIC1)
	{
		netWave[cell->opt_net_id[0]].initialValue = Wave::H;
		return;
	}

	// assnign logic to 0 to cell LOGIC0
	else if(cell->logic_type == LOGIC0)
	{
		netWave[cell->opt_net_id[0]].initialValue = Wave::L;
		return;
	}
	
	// final fanout initial value for the target cell
	int val = netWave[cell->ipt_net_id[0]].initialValue;

	// used for xor and xnor
	bool odd = false;

	// visit the fanin of the cell to determind the fanout
	for(unsigned i = 0 ; i < cell->ipt_net_id.size() ; ++i)
	{
		int netID = (cell->ipt_net_id[i]);

		if(netID == -1)
			continue;

		int netVal = netWave[netID].initialValue;

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
	
	// assign the final value to fanout of cell
	netWave[cell->opt_net_id[0]].initialValue = val;
	return;
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
		int ci_value = (wave.transition.size() == 0)?wave.initialValue:wave.transition.back().value;
		//cout << "check net " << cir->nets[netID].name << endl;
		if(ci_value != value)
		{
			//cout << "po with diff value: " << cir->nets[netID].name << " should be: " << value<< endl;
			//netWave[netID].print();
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
			int pinD = lib->getFaninIndex(cir->cells[cellID].type, cir->dff_d);
			int netD = cir->cells[cellID].ipt_net_id[pinD];
			Wave &wave = netWave[netD];
			int sim_value = (wave.transition.size() ==0)? wave.initialValue:wave.transition.back().value;
			DBG("check net " << cir->nets[netD].name << endl );
			if(sim_value != value)
			{
				//cout << "ppo with diff value: " << cir->nets[netD].name << " should be: " << value<< endl;
				return false;
			}
		}
		else
		{
			int pinQ = lib->getFanoutIndex(cir->cells[cellID].type, cir->dff_q);
			int netQ = cir->cells[cellID].opt_net_id[pinQ];
			assert(netQ != -1);
			Wave &wave = netWave[netQ];
			DBG("check net " << cir->nets[netQ].name << endl );
			int sim_value = (wave.transition.size() == 0)?wave.initialValue:wave.transition.back().value;
			if(sim_value != value)
			{
				//cout << sim_value << endl;
				//cout << "ppo with diff value: " << cir->nets[netQ].name << " should be: " << value<< endl;
				return false;
			}
		}
	}
	for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
		for(unsigned j = 0 ; j < netWave[i].transition.size() ; ++j)
			netWave[i].transition[j].netID = i;

	return true;
}

std::string CircuitSimulator::getPPO()
{
	string ret;
	for(unsigned i = 0 ; i < ppiOrder.size() ; i++)
	{
		int cellID = ppiOrder[i];
		int pinD = lib->getFaninIndex(cir->cells[cellID].type, cir->dff_d);
		int netD = cir->cells[cellID].ipt_net_id[pinD];
		assert(netD != -1);
		Wave &wave = netWave[netD];
		int sim_value = (wave.transition.size() == 0)?wave.initialValue:wave.transition.back().value;
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
