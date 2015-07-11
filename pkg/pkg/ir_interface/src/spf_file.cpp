#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include "spf_file.h"

using namespace std;
using namespace IntfNs;


SpfFile::SpfFile(){}
SpfFile::~SpfFile(){}

bool SpfFile::read(const char* const fileName)
{
	ifstream fin(fileName);
	
	if(!fin)
	{
		cout << "**ERROR SpfFile::read - can't open: " << fileName <<endl;
		return false;
	}

	char delim;
	string operation;

	while(fin >> operation)
	{
		if(operation == "*|NET")
		{
			string netName;
			double capacitance;
			fin >> netName >> capacitance;
			addNet(netName , capacitance);
		}
		else if(operation == "*|P")
		{
			string pinName;
			char ioType;
			double capacitance;
			double x;
			double y;
			fin >> delim >>pinName >> ioType >> capacitance;
			if(fin.peek() == 'P')//PF
			{
				capacitance/=pow(10,12);
				fin >> delim >> delim;
			}
			fin >> x >> y >> delim;
			addPin(pinName , ioType , capacitance , x , y);
		}
		else if(operation == "*|I")
		{
			string instancePinName;
			string instanceName;
			string pinName;
			char ioType;
			double capacitance;
			double x;
			double y;
			fin >> delim >> instancePinName >> instanceName >> pinName >> ioType >> capacitance;
			if(fin.peek() == 'P')//PF
			{
				capacitance/=pow(10,12);
				fin >> delim >> delim;
			}
			fin >> x >> y >> delim;
			addInstancePin(instancePinName , instanceName , pinName , ioType , capacitance , x , y);
		}
		else if(operation == "*|S")
		{
			string nodeName;
			double x;
			double y;
			fin >> delim >> nodeName >> x >> y >> delim;
			addSubNode(nodeName , x , y);
		}
		else if(operation[0] == 'R')
		{
			string resistorName = operation;
			string nodeName1;
			string nodeName2;
			double value;
			fin >> nodeName1 >> nodeName2 >> value;
			addResistor(resistorName , nodeName1 , nodeName2 , value);
		}
		else if(operation[0] == 'C')
		{
			string capacitanceName = operation;
			string nodeName1;
			string nodeName2;
			double value;
			fin >> nodeName1 >> nodeName2 >> value;
			if(fin.peek() == 'P')//PF
			{
				value/=pow(10,12);
				fin >> delim >> delim;
			}
			addCapacitance(capacitanceName , nodeName1 , nodeName2 , value);
		}
		else if(operation[0] == 'L')
		{
			string lName = operation;
			string nodeName1;
			string nodeName2;
			double value;
			fin >> nodeName1 >> nodeName2 >> value;
			addInductance(lName , nodeName1 , nodeName2 , value);
		}
		else if(operation[0] == 'V')
		{
			string nodeName;
			string delim;
			double voltage;
			fin >> nodeName >> delim >> voltage;
			addIndependentVoltageSource(nodeName , voltage);
		}
		else
		{
			fin.ignore(1024 , '\n');//ignore until '\n'
		}
	}
	return true;
}

bool SpfFile::addNet(const string& netName , double capacitance)
{
	cout << "netName: " << netName << " cap: " << capacitance<<endl;
	return true;
}

bool SpfFile::addPin(const string& pinName , char ioType , double capacitance , double x , double y)
{
	cout << "pinName: " << pinName << " io: " << ioType << " cap: " << capacitance << " x: " << x << " y: " << y <<endl;
	return true;
}

bool SpfFile::addInstancePin(const string& instancePinName , const string& instanceName, const string& pinName , char ioType , double capacitance , double x , double y)
{
	cout << "instancePinName: " << instancePinName << " instanceName: " << instanceName << " pinName: " << pinName << " io: " << ioType << " cap: " << capacitance << " x: " << x << " y: " << y <<endl;
	return true;
}

bool SpfFile::addSubNode(const string& subNodeName , double x , double y)
{
	cout << "subNodeName: " << subNodeName << " x: " << x << " y: " << y <<endl;
	return true;
}

bool SpfFile::addResistor(const string& resistorName , const string& nodeName1 , const string& nodeName2 , double value)
{
	cout << "res: " << resistorName << " node: " << nodeName1 << " node: " << nodeName2 << " value: " << value << endl;
	return true;
}

bool SpfFile::addCapacitance(const string& capacitanceName , const string& nodeName1 , const string& nodeName2 , double value)
{
	cout << "cap: " << capacitanceName << " node: " << nodeName1 << " node: " << nodeName2 <<" value: " << value << endl;
	return true;
}

bool SpfFile::addInductance(const std::string &inductanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value)
{
	cout << "ind: " << inductanceName << " node:" << nodeName1 << " node2" << nodeName2 <<" value: " << value << endl;
	return true;
}
bool SpfFile::addIndependentVoltageSource(const string& nodeName , double value)
{
	cout << "node: " << nodeName << "supply voltage: " << value <<endl;
	return true;
}

