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
class Transition
{
public:

	// used by simulator
	double time;
	double period;
	int value;
	const Transition *prevTransition;
	
	// used by idea
	double gateDelay;
	double extraGateDelay;
	double vh;
	double vl;
	int netID;

};
class Wave
{
public:

	enum Value{
		L = 0,
		H = 1,
		X = 3
	};
	static int invert(int v);
	Wave& operator= (const Wave &t);
	Wave& copyAndLinkTransition(const Wave &t);


	Wave():initialValue(X),size(0){}
	void addDelay(double riseDelay , double fallDelay);
	void addPeriod(int cellType , int ipt , int opt , double riseCap , double fallCap , Library *lib);
	void _and(Wave &w);
	void _or(Wave &w);
	void _xor(Wave &w);
	void inv();
	void addTransition(int v, double time , double period, const Transition *prev, double gateDelay=0);
	void mergeWave(bool invert, int controlValue, Wave &wave);
	void print();

	int initialValue;
	unsigned size;
	
	Transition transition[TRANSITIONS];
};



#endif
