#include "spMaker.h"
#include "stdio.h"
#include "stdlib.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

SpMaker::SpMaker(const std::string &fName):fout(fName.c_str())
{
	vsAmount = 0;
	this->fName = fName;
	simType = NOT_YET;
	std::ios::sync_with_stdio(false);
}

void SpMaker::add(const std::string &line)
{
	fout << line << std::endl;
}


void SpMaker::setMeasure(const std::string &node1 ,double value1, bool rise1, 
                         const std::string &node2, double value2, bool rise2 , const std::string& measureName)
{
	
//.meas TRAN rising_delay trig v(INPUT) val=0.55 rise=1 targ v(OUTPUT) val=0.55 fall=1
	std::string transition1 = (rise1)?"rise":"fall";
	std::string transition2 = (rise2)?"rise":"fall";
	fout << ".meas TRAN " << measureName;
	fout << " TRIG v(" << node1 << ") VAL=" << value1 << " " << transition1<< "=last ";
	fout << " TARG v(" << node2 << ") VAL=" << value2 << " " << transition2<< "=last " << std::endl;
}


void SpMaker::addAlter(const std::string &line)
{
	fout << ".alter " ;
	std::string lineline ( line);
	add(lineline);
}

bool SpMaker::spiceSim()
{
	fout << ".end" << std::endl;
	const std::string command = "hspice -i " + fName + " -mt 8 -hpp -o " + fName + ".lis";
	system(command.c_str());
	simType = SPICE;
	return true;
}
bool SpMaker::nanoSim()
{
	fout << ".end" << std::endl;
	const std::string command = "nanosim -nspi " + fName + " -o "+fName+"_output -A -out fsdb -c ./verify/sp.cfg -t ns";
	system(command.c_str());
	simType = NANO_SIM;
	return true;
}
Measurement SpMaker::getMeasure(const std::string &measureName)
{
	if(simType == NANO_SIM)
		return getMeasureNano(measureName);
	else if(simType == SPICE)
		return getMeasureSpice(measureName);
	else
	{
		cout << "**ERROR SpMaker::getMeasure: please perform simulation before get measurement" << endl;
		return Measurement();
	}
}

std::vector<Measurement> SpMaker::getAlterMeasure(const std::string &measureName)
{
	if(simType == NANO_SIM)
		return getAlterMeasureNano(measureName);
	else if(simType == SPICE)
		return getAlterMeasureSpice(measureName);
	else
	{
		cout << "**ERROR SpMaker::getMeasure: please perform simulation before get measurement" << endl;
		return vector<Measurement>();
	}
}

Measurement SpMaker::getMeasureSpice(const std::string &measureName)
{
	const std::string lisName = fName + ".lis";
	std::ifstream fin(lisName.c_str());
	if(!fin)
	{
		std::cout << "**ERROR: can't find lis file" << std::endl;
		Measurement bad;
		return bad;
	}
	std::string mName = measureName + "=";
	transform(mName.begin() , mName.end() , mName.begin() , ::tolower);
	std::string in;
	Measurement ret;
	while(fin >> in)
		if(in == mName)
		{
			fin >> std::ws;
			if(fin.peek() == 'f')// fail to read measurement
			{
				ret.gateDelay = 0;
				ret.endTime = 0;
				ret.startTime = 0;
			}
			else
				fin >> ret.gateDelay >> in >> ret.startTime >> in >> in >> ret.endTime;
			if(ret.startTime > ret.endTime)
				swap(ret.startTime , ret.endTime);
			break;
		}
	return ret;
}

Measurement SpMaker::getMeasureNano(const std::string &measureName)
{
	cout << "get measurement nano-----------" << endl;
	const std::string lisName = "output.meas";
	std::ifstream fin(lisName.c_str());
	if(!fin)
	{
		std::cout << "**ERROR: can't find lis file" << std::endl;
		Measurement bad;
		return bad;
	}
	std::string mName = measureName + "=";
	transform(mName.begin() , mName.end() , mName.begin() , ::tolower);
	std::string in;
	Measurement ret;
	while(fin >> in)
		if(in == mName)
		{
			fin >> std::ws;
			if(fin.peek() == 'f')// fail to read measurement
			{
				ret.gateDelay = 0;
				ret.endTime = 0;
				ret.startTime = 0;
			}
			else
				fin >> in >> ret.gateDelay >> in >> in >> ret.startTime >> in >> in >> ret.endTime;
			if(ret.startTime > ret.endTime)
				swap(ret.startTime , ret.endTime);
			break;
		}
	return ret;
}

std::vector<Measurement> SpMaker::getAlterMeasureSpice(const std::string &measureName)
{
	std::vector<Measurement> ret;
	const std::string lisName = fName + ".lis";
	std::ifstream fin(lisName.c_str());
	if(!fin)
	{
		std::cout << "**ERROR: can't find lis file: " << lisName << std::endl;
		return ret;
	}
	std::string mName = measureName + "=";
	transform(mName.begin() , mName.end() , mName.begin() , ::tolower);
	std::string in;
	while(fin >> in)
		if(in == mName)
		{
			Measurement met;
			fin >> std::ws;
			if(fin.peek() == 'f')// fail to read measurement
			{
				met.gateDelay = 0;
				met.endTime = 0;
				met.startTime = 0;
			}
			else
				fin >> met.gateDelay >> in >> met.startTime >> in >> in >> met.endTime;
			if(met.startTime > met.endTime)
				swap(met.startTime , met.endTime);
			ret.push_back(met);
		}
	return ret;
}


std::vector<Measurement> SpMaker::getAlterMeasureNano(const std::string &measureName)
{
	//cout << "get alter measure nano" << endl;
	//cout << "find word name: " << measureName << endl;
	std::vector<Measurement> ret;
	const std::string lisName = fName + "_output.meas";
	int count(0);
	//cout << "open file: " << lisName << endl;
	std::ifstream fin(lisName.c_str());
	while(fin)
	{
		std::string mName = measureName;
		transform(mName.begin() , mName.end() , mName.begin() , ::tolower);
		std::string in;
		while(fin >> in)
		{
			if(in == mName)
			{
				Measurement met;

				fin >> std::ws;
				if(fin.peek() == 'f')// fail to read measurement
				{
					met.gateDelay = 0;
					met.endTime = 0;
					met.startTime = 0;
				}
				else
					fin >> in >> met.gateDelay >> in >> in >> met.endTime >> in >> in >> met.startTime;
				if(met.startTime > met.endTime)
					swap(met.startTime , met.endTime);
				/*
				cout << "  period: " << met.gateDelay << endl;
				cout << "  start: " << met.startTime << endl;
				cout << "  end: " << met.endTime << endl;
				*/

				ret.push_back(met);
			}
		}
		count++;
		fin.close();
		fin.open((lisName+char('0'+count)).c_str());
		//cout << "open file: " << lisName+char('0'+count) << endl;
	}
	//cout << "fail to open" << endl;
	return ret;
}
