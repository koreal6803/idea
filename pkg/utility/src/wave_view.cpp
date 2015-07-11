#include <fstream>
#include <cassert>
#include <algorithm>
#include "wave_view.h"
#include "plot.h"
using namespace std;


WaveView::WaveView(const char *fName)
{
	ifstream fin(fName);

	int timeStepAmount;
	string in;
	
	// read max path delay, time step, time step amount
	fin >> in >> in;
	fin >> in >> timeStep;
	fin >> in >> timeStepAmount;

	fin >> in;

	// parse current data
	int cnt = 0;
	while(fin >> in)
	{
		if(in == "voltageData")
			break;

		assert(in == "nodeCurrent");
		
		string nodeName;
		fin >> nodeName;
		transform(nodeName.begin() , nodeName.end() , nodeName.begin(), ::tolower);
		nodeIDs[nodeName] = cnt++;
		
		vector<double> vals;
		for(int i = 0 ; i < timeStepAmount ; i++)
		{
			double val;
			fin >> val;
			vals.push_back(val);
		}
		currents.push_back(vals);
	}

	// parse voltage data
	while(fin >> in)
	{
		assert(in == "nodeVoltage");
		
		string nodeName;
		fin >> nodeName;

		nodeIDs[nodeName] = voltages.size();
		transform(nodeName.begin() , nodeName.end() , nodeName.begin(), ::tolower);
		vector<double> vals;
		for(int i = 0 ; i < timeStepAmount ; i++)
		{
			double val;
			fin >> val;
			vals.push_back(val);
		}
		voltages.push_back(vals);
	}
}

bool WaveView::writeVoltage(const std::string nodeName , const std::string &optFile)
{
	string nName(nodeName);
	transform(nName.begin() , nName.end() , nName.begin(), ::tolower);

	if(nodeIDs.find(nName)==nodeIDs.end())
		return false;

	cout << "find: " << nodeIDs.find(nName)->first << endl;

	Plot plot(optFile);
	plot.setTitle(nodeName);
	plot.setXName("time(ns)");
	plot.setYName("voltage(v)");

	int nodeID = nodeIDs[nodeName];
	cout << "nodeID " << nodeID << endl;

	vector<Point> vp;
	for(unsigned i = 0 ; i < voltages[nodeID].size() ; ++i)
	{
		Point p1,p2;
		p1.y = voltages[nodeID][i];
		p2.y = voltages[nodeID][i];
		p1.x = i * timeStep;
		p2.x = (i + 1) * timeStep;
		vp.push_back(p1);
		vp.push_back(p2);
		cout << voltages[nodeID][i] << " ";
	}

	plot.addSet("voltage",vp);
	plot.draw();
	return true;
}

