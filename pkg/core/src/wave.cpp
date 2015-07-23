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

/*
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
*/

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
	for(unsigned i = 0 ; i < transition.size() ; ++i)
	{
		switch(transition[i].value)
		{
			case Wave::H:
				transition[i].time+=riseDelay;
				break;
			case Wave::L:
				transition[i].time+=fallDelay;
				break;
			case Wave::X:
				transition[i].time+=(riseDelay + fallDelay)*0.5;
		}
	}
	
	// deal with hazard
	for(unsigned i = 1 ; i < transition.size() ; ++i)
	{
		if(transition[i].time <= transition[i-1].time + 0.01){
			// remove i and i-1 th hazard
			//transition.erase(transition.begin() + i-1, transition.begin()+i+1);
		}
	}
}
void Wave::addDelay(int cellType, int ipt, int opt, double riseCap, double fallCap, Library *lib)
{
	LookUpTableInfo info;
	info.returnType = PROPAGATION_DELAY;
	info.cellIndex = cellType;
	info.pinIndex = opt;
	info.relativePinIndex = ipt;
	info.faninSignal = 0;
	double riseDelay,fallDelay;
	/*
	cout << "lookup table: " << endl;
	cout << " cell type: " << cellType << endl;
	cout << " ipt pin : " << ipt << endl;
	cout << " opt pin : " << opt << endl;
*/
	for(unsigned i = 0 ; i < transition.size() ; ++i)
	{
		info.inputTransition = transition[i].period;
//		cout << " trans: " << info.inputTransition << endl;
		switch(transition[i].value)
		{
			case Wave::H:
				info.toggleRise = true;
				info.capacitance = riseCap;
//				cout << " rise: " << info.toggleRise << endl;
//				cout << " cap: " << info.capacitance << endl;
				riseDelay = lib->get(info);
				transition[i].time += riseDelay;
//				cout << " delay: " << riseDelay << endl;
				break;
			case Wave::L:
				info.toggleRise = false;
				info.capacitance = fallCap;
//				cout << " rise: " << info.toggleRise << endl;
//				cout << " cap: " << info.capacitance << endl;
				fallDelay = lib->get(info);
				transition[i].time += fallDelay;
//				cout << " delay: " << fallDelay << endl;
				break;
			case Wave::X:
				assert(0);
		}
	}

	// deal with hazard
	for(unsigned i = 1 ; i < transition.size() ; ++i)
	{
		if(transition[i].time <= transition[i-1].time + 0.01){
			// remove i and i-1 th hazard
			// transition.erase(transition.begin() + i-1, transition.begin()+i+1);
		}
	}
}

void Wave::addPeriod(int cellType , int ipt , int opt , double riseCap , double fallCap, Library *lib)
{
	LookUpTableInfo info;
	info.returnType = TRANSITION_TIME;
	info.cellIndex = cellType;
	info.pinIndex = opt;
	info.relativePinIndex = ipt;
	info.faninSignal = 0;

	for(unsigned i = 0 ; i < transition.size() ; ++i)
	{
		info.inputTransition = transition[i].period;
		info.toggleRise = transition[i].value == H;
		info.capacitance = (info.toggleRise)? riseCap : fallCap;
		transition[i].period = lib->get(info);
	}
}

void Wave::_buf(Wave &w , int iptID)
{
	
	initialValue = w.initialValue;
	transition = w.transition;
	for(unsigned i = 0 ; i < w.transition.size() ; ++i)
	{
		transition[i].prevTransition = &w.transition[i];
		transition[i].iptPin = iptID;
	}
}

void Wave::_and(Wave &w , int iptID)
{
	// mergeWave(controlValue , wave , iptID)
	mergeWave(0,w,iptID);
}

void Wave::_or(Wave &w , int iptID)
{
	// mergeWave(controlValue , wave , iptID)
	mergeWave(1,w,iptID);
}
/*
void Wave::_xor(Wave &w , int iptID)
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
		//cout << "tt: " << tt1 << " " << tt2 <<endl;
		if(tt1 < tt2)
		{
			//cout << " case 1" <<endl;
			opt = invert(opt);
			newWave.addTransition(opt , transition[pos1]);//tt1, tp1, p1, d1);
			newWave.transition[newWave.size-1].gateIptPin = cellIptPinID;
			pos1++;
		}
		else if(tt2 < tt1)
		{
			//cout << " case 2" <<endl;
			opt = invert(opt);
			newWave.addTransition(opt , w.transition[pos2]);//tt2, tp2, p2, d2);
			newWave.transition[newWave.size-1].gateIptPin = w.cellIptPinID;
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
		opt = invert(opt);
		newWave.addTransition(opt , transition[pos1]);
		newWave.transition[newWave.size-1].gateIptPin = cellIptPinID;
	}
	for(;pos2 < w.size ; ++pos2)
	{
		opt = invert(opt);
		newWave.addTransition(opt , w.transition[pos2]);
		newWave.transition[newWave.size-1].gateIptPin = w.cellIptPinID;
	}
	swap(newWave , *this);
	//print();
	//cout << "===========================" <<endl;
}
*/
void Wave::inv()
{
	initialValue = invert(initialValue);
	for(unsigned i = 0 ; i < transition.size() ; i++)
		transition[i].value = invert(transition[i].value);
}

void Wave::mergeWave(int controlValue, const Wave &w , int iptID)
{
	/*
	cout << "===================start merge====================" << endl;
	if(controlValue == 1)
		cout << "                       OR                         " << endl;
	else
		cout << "                       AND                         " << endl;
	cout << "-------------------transition1" << endl;
	print();
	cout << "-------------------transition2" << endl;
	cout << "-------------------start merge" << endl;
	w.print();//*/

	
	
	int pv1 = initialValue;
	int pv2 = w.initialValue;

	int lastValue1 = (transition.empty())? initialValue : transition.back().value;
	int lastValue2 = (w.transition.empty())? w.initialValue : w.transition.back().value;

	if(controlValue)
		initialValue = initialValue | w.initialValue;
	else
		initialValue = initialValue & w.initialValue;

	

	unsigned pos1 = 0;
	unsigned pos2 = 0;

	while(pos1 < transition.size() || pos2 < w.transition.size())
	{
		int v1 = (pos1 < transition.size())? transition[pos1].value : lastValue1;
		int v2 = (pos2 < w.transition.size())? w.transition[pos2].value : lastValue2;
		double t1 = (pos1 < transition.size())?transition[pos1].time : 1e30;
		double t2 = (pos2 < w.transition.size())?w.transition[pos2].time : 1e30;
		/* cout << "v1: " << v1 << " " << t1 << endl;
		 cout << "v2: " << v2 << " " << t2 << endl;//*/
		if(t1 == t2)
		{
			//cout << "t1 == t2" << endl;

			bool outputToggle = v1 == v2;
			//cout << "  outputToggle = " << outputToggle << endl;
			if(outputToggle)
			{
				/*cout << "  pos1++" << endl;
				cout << "  pos2++" << endl;//*/
				pos1++;
				pos2++;
			}
			else
			{
				/*cout << "  erase pos1" << endl;
				cout << "  pos2++" << endl;//*/
				transition.erase(transition.begin() + pos1);
				pos2++;
			}
			pv1 = v1;
			pv2 = v2;
		}
		else if(t1 < t2)
		{
			// cout << "t1 < t2" << endl;
			// t1 is the active transition
			// t2 will transition later
			bool outputToggle = pv2 != controlValue;
			/*cout << "  outputToggle = " << outputToggle << endl;
			cout << "  pv2 " << pv2 << endl;//*/
			if(outputToggle)
			{
				// save the transition
				// cout << "  pos1++" << endl;
				pos1++;
			}
			else
			{
				// remove the transition
				// cout << "  erase pos1" << endl;
				transition.erase(transition.begin() + pos1);
			}
			pv1 = v1;
		}
		else
		{
			// cout << "t2 < t1" << endl;
			bool outputToggle = pv1 != controlValue;
			/* cout << "  outputToggle = " << outputToggle << endl;
			cout << "  pv1 " << pv1 << endl;//*/
			if(outputToggle)
			{
				// save the transition
				transition.insert(transition.begin() + pos1 , w.transition[pos2]);
				transition[pos1].prevTransition = &w.transition[pos2];
				transition[pos1].iptPin = iptID;
				pos1++;
			}
			pos2++;
			pv2 = v2;
		}
	}


	//cout << "-------------------final result" << endl;
	//print();
	if(transition.size())
		assert(initialValue != transition[0].value);
	for(unsigned i = 1 ; i < transition.size() ; ++i)
		assert(transition[i].value != transition[i-1].value);
	//getchar();
	

	return;
}

void Wave::addTransition(int v , const Transition &trans)
{
	if(transition.empty())
		assert(v != initialValue);
	else
		assert(v != transition.back().value);
	transition.push_back(trans);
	transition.back().value = v;
}


void Wave::print()const
{
	cout <<  "wave-----------------------" <<endl;
	cout << " init val = " << initialValue <<endl;
	for(unsigned i = 0 ; i < transition.size() ; ++i)
		cout <<  " val = " << transition[i].value << " t = " << transition[i].time <<endl;
}

void Wave::propagate(int cellType, int opt, double riseCap, double fallCap, Library *lib)
{
	LookUpTableInfo info;
	info.cellIndex = cellType;
	info.pinIndex = opt;
	info.faninSignal = 0;
	for(int i = transition.size()-1 ; i >= 0 ; i--)
	{
		// update transition
		info.returnType = TRANSITION_TIME;
		info.inputTransition = transition[i].period;
		info.relativePinIndex = transition[i].iptPin;
		assert(info.relativePinIndex != -1);
		if(lib->getFaninSize(cellType) == 1){
			info.relativePinIndex = 0;
		}
		info.toggleRise = transition[i].value == H;
		info.capacitance = (info.toggleRise)? riseCap : fallCap;
		transition[i].period = lib->get(info);

		// update gate delay
		info.returnType = PROPAGATION_DELAY;
		transition[i].time += lib->get(info);

		// false hazard
		if(i+1 < (int)transition.size() && transition[i+1].time < transition[i].time+1.25*transition[i].period)
			transition.erase(transition.begin()+i , transition.begin()+i+2);
	}
	//for(unsigned i = 1 ; i < transition.size() ; ++i)
	//	assert(transition[i].time > transition[i+1].time);
	
	for(int i = transition.size()-1 ; i >= 0 ; i--)
		if(i+1 < (int)transition.size() && transition[i+1].time < transition[i].time + 0.005)
			transition.erase(transition.begin()+i , transition.begin()+i+2);
	
}
