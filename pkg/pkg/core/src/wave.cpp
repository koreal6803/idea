#include"wave.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
#include "library.h"
#ifdef USEDEBUG
#define DBG(x) std::cout << x
#else
#define DBG(x) 
#endif 


using namespace std;


Wave& Wave::operator= (const Wave &t)
{
	initialValue = t.initialValue;
	size = t.size;
	for(unsigned i = 0 ; i < t.size ; ++i)
		transition[i] = t.transition[i];
	return *this;
}


Wave& Wave::copyAndLinkTransition(const Wave &t)
{
	initialValue = t.initialValue;
	size = t.size;
	for(unsigned i = 0 ; i < t.size ; ++i)
		transition[i] = t.transition[i];
	for(unsigned i = 0 ; i < t.size ; ++i)
		transition[i].prevTransition = &t.transition[i];
	return *this;
}

int Wave::invert(int v)
{
	switch(v)
	{
		case H:
			return L;
		case L:
			return H;
		case X:
			return X;
	}
	return X;
}

void Wave::addDelay(double riseDelay , double fallDelay)
{
	for(unsigned i = 0 ; i < size ; ++i)
	{
		switch(transition[i].value)
		{
			case Wave::H:
				transition[i].time+=riseDelay;
				transition[i].gateDelay = riseDelay;
				break;
			case Wave::L:
				transition[i].time+=fallDelay;
				transition[i].gateDelay = fallDelay;
				break;
			case Wave::X:
				transition[i].time+=(riseDelay + fallDelay)*0.5;
				transition[i].gateDelay = (riseDelay + fallDelay) * 0.5;
		}
	}
	bool first = true;
	for(unsigned i = 1 ; i < size ; ++i)
	{
		if(transition[i].time <= transition[i-1].time){
			if(first)
			{
				first = false;
				//cout << "==============add delay================" << endl;
				//print();
			}
			for(unsigned j = i+1 ; j < size ; ++j)
			{
				transition[j-2] = transition[j];
			}
			i--;
			size-=2;
		}
	}
	//if(first == false)
	//	print();
}

void Wave::addPeriod(int cellType , int ipt , int opt , double riseCap , double fallCap, Library *lib)
{
	LookUpTableInfo info;
	info.returnType = TRANSITION_TIME;
	info.cellIndex = cellType;
	info.pinIndex = opt;
	info.relativePinIndex = ipt;
	info.faninSignal = 0;

	for(unsigned i = 0 ; i < size ; ++i)
	{
		info.inputTransition = transition[i].period;
		info.toggleRise = transition[i].value == H;
		info.capacitance = (info.toggleRise)? riseCap : fallCap;
		transition[i].period = lib->get(info);
	}
}

void Wave::_and(Wave &w)
{
	// DBG( "AND===============================AND" <<endl);
	// DBG ( "================start================" <<endl);
	mergeWave(false,L,w);
	// DBG ( "0000000000000000000000000000000000000" <<endl);
}

void Wave::_or(Wave &w)
{
	// DBG ( "OR=================================OR" <<endl);
	// DBG ( "================start================" <<endl);
	mergeWave(false,H,w);
	// DBG ( "0000000000000000000000000000000000000" <<endl);
}

void Wave::_xor(Wave &w)
{
	int opt;
	if(initialValue != w.initialValue)
		opt = Wave::H;
	else if(initialValue == w.initialValue)
		opt = Wave::L;
	if(initialValue == Wave::X || w.initialValue == Wave::X)
		opt = Wave::X;
	//cout << "XORXOR==============XORXOR" <<endl;
	//print();
	//w.print();
	unsigned pos1 = 0;
	unsigned pos2 = 0;
	Wave newWave;
	newWave.initialValue = opt;
	//cout << "pos1 " << size <<endl;
	//cout << "pos2 " << w.size <<endl;
	
	while(pos1 < size && pos2 < w.size)
	{
		double tt1 = transition[pos1].time;
		double tt2 = w.transition[pos2].time;
		double tp1 = transition[pos1].period;
		double tp2 = w.transition[pos2].period;
		const Transition *p1 = transition[pos1].prevTransition;
		const Transition *p2 = w.transition[pos2].prevTransition;
		double d1 = transition[pos1].gateDelay;
		double d2 = w.transition[pos2].gateDelay;
		//cout << "tt: " << tt1 << " " << tt2 <<endl;
		if(tt1 < tt2)
		{
			//cout << " case 1" <<endl;
			opt = invert(opt);
			newWave.addTransition(opt , tt1, tp1, p1, d1);
			pos1++;
		}
		else if(tt2 < tt1)
		{
			//cout << " case 2" <<endl;
			opt = invert(opt);
			newWave.addTransition(opt , tt2, tp2, p2, d2);
			pos2++;
		}
		else
		{
			//cout << " case 3" <<endl;
			pos1++;
			pos2++;
		}
	}

	for(;pos1 < size ; ++pos1)
	{
		double tt1 = transition[pos1].time;
		double tp1 = transition[pos1].period;
		const Transition *p1 = transition[pos1].prevTransition;
		double d1 = transition[pos1].gateDelay;
		
		opt = invert(opt);
		newWave.addTransition(opt , tt1, tp1 , p1 , d1);
	}
	for(;pos2 < w.size ; ++pos2)
	{
		double tt2 = w.transition[pos2].time;
		double tp2 = w.transition[pos2].period;
		const Transition *p2 = w.transition[pos2].prevTransition;
		double d2 = w.transition[pos2].gateDelay;
		//cout << "add t2: " << pos2 << " " << tt2 << endl;
		opt = invert(opt);
		newWave.addTransition(opt , tt2, tp2, p2, d2);
	}
	swap(newWave , *this);
	//print();
	//cout << "===========================" <<endl;
}

void Wave::inv()
{
	initialValue = invert(initialValue);
	for(unsigned i = 0 ; i < size ; i++)
		transition[i].value = invert(transition[i].value);
}

void Wave::mergeWave(bool invert, int controlValue, Wave &w)
{
	int val;
	int nonControlValue = (controlValue == Wave::H)? Wave::L:Wave::H;
	if(initialValue == controlValue or w.initialValue == controlValue)
		val = (invert)? nonControlValue :controlValue;
	else if(initialValue == Wave::X and initialValue == X)
		val = Wave::X;
	else
		val = (invert)? controlValue : nonControlValue;

	unsigned pos1 = 0;
	unsigned pos2 = 0;
	bool diff;
	int val1 = initialValue;
	int val2 = w.initialValue;

	Wave newWave;
	newWave.initialValue = val;
	while(pos1 < size && pos2 < w.size)
	{
		if(transition[pos1].time == w.transition[pos2].time)
		{
			//DBG ( "case 1" <<endl);
			val1 = transition[pos1].value;
			val2 = w.transition[pos2].value;
			diff = ((invert && val2 == val) || (!invert && val2 != val));
			if(val1 == val2 && diff){
				val = this->invert(val);
				newWave.addTransition(val, transition[pos1].time, transition[pos1].period, transition[pos1].prevTransition , transition[pos1].gateDelay);
				//DBG ( "add transition: " << val << " " << transitionTime[pos1]<<endl);
			}
			pos1++;
			pos2++;
		}
		else if(transition[pos1].time > w.transition[pos2].time)
		{
			//DBG ( "case 2" <<endl);
			val2 = w.transition[pos2].value;
			diff = ((invert && val2 == val) || (!invert && val2 != val));
			if(diff && (val2 == controlValue || val1 == val2))
			{
				val = this->invert(val);
				newWave.addTransition(val, w.transition[pos2].time, w.transition[pos2].period, w.transition[pos2].prevTransition, w.transition[pos2].gateDelay);
				//DBG ( "add transition: " << val << " " << w.transitionTime[pos2]<<endl);
			}
			pos2++;
		}
		else
		{
			//DBG ( "case 3" <<endl);
			val1 = transition[pos1].value;
			diff = ((invert && val1 == val) || (!invert && val1 != val));
			if(diff && (val1 == controlValue || val2 == val1))
			{
				val = this->invert(val);
				newWave.addTransition(val, transition[pos1].time, transition[pos1].period, transition[pos1].prevTransition, transition[pos1].gateDelay);
				//DBG ( "add transition: " << val << " " << transitionTime[pos1]<<endl);
			}
			pos1++;
		}
	}

	if(pos1 < size && val2 != controlValue)
		for(;pos1 < size;++pos1)
		{
			//DBG ( "pos1 : " << pos1 <<endl);
			val1 = transition[pos1].value;
			diff = ((invert && val1 == val) || (!invert && val1 != val));
			if(diff)
			{
				val = this->invert(val);
				newWave.addTransition(val, transition[pos1].time, transition[pos1].period, transition[pos1].prevTransition, transition[pos1].gateDelay);
				//DBG ( "add transition: " << val << " " << transitionTime[pos1]<<endl);
			}
		}
	
	if(pos2 < w.size && val1 != controlValue)
		for(;pos2 < w.size;++pos2)
		{
			//DBG ( "pos2 : " << pos2 << endl);
			//DBG ( "size : " << w.size <<endl);
			val2 = w.transition[pos2].value;
			diff = ((invert && val2 == val) || (!invert && val2 != val));
			if(diff)
			{
				val = this->invert(val);
				newWave.addTransition(val, w.transition[pos2].time, w.transition[pos2].period, w.transition[pos2].prevTransition, w.transition[pos2].gateDelay);
				//DBG ( "add transition: " << val2 << " " << w.transitionTime[pos2]<<endl);
			}
		}

	swap((*this) , newWave);
	//DBG ( "result" <<endl);
	//this->print();
	return;
}

void Wave::addTransition(int v , double t , double p ,  const Transition *prev , double gateDelay)
{
	assert(size+1 < TRANSITIONS);
	transition[size].time = t;
	transition[size].value = v;
	transition[size].period = p;
	transition[size].prevTransition = prev;
	transition[size].gateDelay = gateDelay;
	size++;
}


void Wave::print()
{
	DBG( "wave-----------------------" <<endl);
	DBG( " init val = " << initialValue <<endl);
	for(unsigned i = 0 ; i < size ; ++i)
		DBG( " val = " << transition[i].value << " t = " << transition[i].time <<endl);
}

