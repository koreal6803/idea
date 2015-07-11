// **************************************************************************
// File       [ wave.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#ifndef __CORE_WAVE_H__
#define __CORE_WAVE_H__

#include <vector>

#define TRANSITIONS 200
class Library;
class Wave;
class Transition
{
public:

	// used by simulator
	Transition():time(0),period(0),value(-1),iptPin(-1),netID(-1),prevTransition(NULL){}
	double time;
	double period;
	int value;
	int iptPin;
	int netID;
	const Transition *prevTransition;

};

class Wave
{
public:

	static int invert(int v);
	// Wave& operator= (const Wave &t);
	enum Value
	{
		H = 1,
		L = 0,
		X = 3
	};

	// operator

	Wave():initialValue(3){
		transition.reserve(50);
	}
	void addDelay(int cellType, int ipt, int opt, double riseCap, double fallCap, Library *lib);
	void addDelay(double riseDelay , double fallDelay);
	void addPeriod(int cellType , int ipt , int opt , double riseCap , double fallCap , Library *lib);
	void _buf(Wave &w , int iptID);
	void _and(Wave &w , int iptID);
	void _or(Wave &w , int iptID);
	void inv();
	void addTransition(int v, const Transition &trans);
	void mergeWave(int controlValue, const Wave &wave, int iptID);
	void print()const ;
	void propagate(int cellType, int opt, double riseCap, double fallCap, Library *lib);
	void print(int indence);

	// data structure

	int initialValue;
	std::vector<Transition> transition;
	
};



#endif
