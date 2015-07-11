#include "power_grid.h"
#include "matrix/src/matrix.h"
#include <iostream>
#include <cassert>

using namespace std;
using namespace pgNs;

PowerGrid::PowerGrid()
{
	nameToIndex["0"] = -1;
	supplyVoltage = -1;
}

PowerGrid::~PowerGrid()
{
}

bool PowerGrid::addNet(const std::string& netName , double capacitance)
{
	return true;
}
bool PowerGrid::addPin(const std::string& pinName , char ioType , double pinCapacitance ,  double x , double y)
{
	return true;
}

bool PowerGrid::addInstancePin(const std::string& instancePinName , const std::string& instanceName, const std::string& pinName , char ioType , double capacitance , double x , double y)
{
	Node::Type type  = (pinName == "VDD")?Node::POWER : Node::GROUND;
	Node newNode;
	newNode.name_ = instancePinName;
	newNode.pad_ = false;
	newNode.type_ = type;
	newNode.x = x;
	newNode.y = y;
	assert(pinName == "VDD" || pinName == "VSS");
	nodes.push_back(newNode);
	nameToIndex[instancePinName] = nodes.size()-1;
	return true;
}

bool PowerGrid::addSubNode(const std::string& subNodeName , double x , double y)
{
	string firstThree(subNodeName.substr(0 , 3));
	Node::Type t = (firstThree == "VDD")?
	Node::POWER:Node::GROUND;
	Node newNode;
	newNode.name_ = subNodeName;
	newNode.pad_ = false;
	newNode.type_ = t;
	newNode.x = x;
	newNode.y = y;
	assert(firstThree == "VDD" || firstThree == "VSS");
	nodes.push_back(newNode);
	nameToIndex[subNodeName] = nodes.size()-1;
	
	return true;
}

bool PowerGrid::addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value)
{
	assert(nameToIndex.find(nodeName1) != nameToIndex.end());
	assert(nameToIndex.find(nodeName2) != nameToIndex.end());
	
	int node1Index = nameToIndex[nodeName1];
	int node2Index = nameToIndex[nodeName2];

	Component newCom;
	newCom.type_ = Component::RES;
	newCom.value_ = value/20;
	newCom.vPos_ = node1Index;
	newCom.vNeg_ = node2Index;
	components.push_back(newCom);

	return true;
}

bool PowerGrid::addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value)
{	
	
	if(nameToIndex.find(nodeName1) == nameToIndex.end()){
		//cout << "*WARN: nodeName " << nodeName1  << " is not found" <<endl;
		//cout << "       (is ok for module pin)" <<endl;
		return true;
	}
	if(nameToIndex.find(nodeName2) == nameToIndex.end()){
		//cout << "*WARN: nodeName " << nodeName2 << " is not found" <<endl;
		//cout << "       (is ok for module pin)" <<endl;
		return true;
	}

	
	int node1Index = nameToIndex[nodeName1];
	int node2Index = nameToIndex[nodeName2];
	
	Component newCom;
	newCom.type_ = Component::CAP;
	newCom.value_ = value;
	newCom.vPos_ = node1Index;
	newCom.vNeg_ = node2Index;
	components.push_back(newCom);
	return true;
}

bool PowerGrid::addInductance(const std::string &lName , const std::string &nodeName1 , const std::string &nodeName2 , double value)
{
	assert(nameToIndex.find(nodeName1) != nameToIndex.end());
	assert(nameToIndex.find(nodeName2) != nameToIndex.end());
	
	int node1Index = nameToIndex[nodeName1];
	int node2Index = nameToIndex[nodeName2];

	Component newCom;
	newCom.type_ = Component::IND;
	newCom.value_ = value;
	newCom.vPos_ = node1Index;
	newCom.vNeg_ = node2Index;
	components.push_back(newCom);
	return true;
}

bool PowerGrid::addIndependentVoltageSource(const string &nodeName , double value)
{
	assert(nameToIndex.find(nodeName) != nameToIndex.end());
	int index = nameToIndex[nodeName];
	nodes[index].pad_ = true;
	nodes[index].voltage_ = value;
	cout << nodeName << value <<endl;
	supplyVoltage = max(value , supplyVoltage);

	Component newCom;
	newCom.type_ = Component::VS;
	newCom.value_ = value;
	newCom.vPos_ = index;
	newCom.vNeg_ = nameToIndex["0"];
	components.push_back(newCom);
	return true;
}

void PowerGrid::addVDD(double vdd)
{
	int rightUpNode = 0;
	for(unsigned i = 0 ; i < nodes.size() ; ++i)
	{
		Node node = nodes[i];
		if(node.type_ != Node::POWER)
			continue;
		if(node.x >= nodes[rightUpNode].x && node.y >= nodes[rightUpNode].y)
			rightUpNode = i;
	}
	addIndependentVoltageSource(nodes[rightUpNode].name_ , vdd);
}

void PowerGrid::addGND(double gnd)
{
	int leftDownNode = 0;
	for(unsigned i = 0 ; i < nodes.size() ; ++i)
	{
		Node node = nodes[i];
		if(node.type_ != Node::GROUND)
			continue;
		if(node.x >= nodes[leftDownNode].x && node.y >= nodes[leftDownNode].y)
			leftDownNode = i;
	}
	addIndependentVoltageSource(nodes[leftDownNode].name_ , gnd);
}

