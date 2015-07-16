#include "power_grid.h"
#include "matrix/src/matrix.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace pgNs;
string modify(const string &name)
{
	string hashName = name;
	std::transform(hashName.begin(), hashName.end(), hashName.begin(), ::tolower);

	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i] == '\\')
			hashName.erase(hashName.begin()+i);
	for(unsigned i = 0 ; i < hashName.size() ; ++i)
		if(hashName[i+1] == '/')
			hashName[i] = '_';
	return hashName;
}
PowerGrid::PowerGrid()
{
	nameToIndex["0"] = -1;
	supplyVoltage = -1;
}

PowerGrid::~PowerGrid()
{
}
int PowerGrid::getPowerNode (const std::string &cellName)
{
	std::string name = modify(cellName + ".VDD");
	if(nameToIndex.find(name)== nameToIndex.end())
		return -1;
	else
		return nameToIndex[name];
}

int PowerGrid::getGroundNode(const std::string &cellName)
{
	std::string name = modify(cellName + ".VSS");
	if(nameToIndex.find(name)== nameToIndex.end())
		return -1;
	else
		return nameToIndex[name];
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
	
	string modifyInstancePinName = modify(instancePinName);
	newNode.name_ = modifyInstancePinName;
	newNode.pad_ = false;
	newNode.type_ = type;
	newNode.x = x;
	newNode.y = y;
	assert(pinName == "VDD" || pinName == "VSS");
	nodes.push_back(newNode);
	nameToIndex[modifyInstancePinName] = nodes.size()-1;
	return true;
}

bool PowerGrid::addSubNode(const std::string& subNodeName , double x , double y)
{
	string firstThree(subNodeName.substr(0 , 3));
	Node::Type t = (firstThree == "VDD")?
	Node::POWER:Node::GROUND;
	Node newNode;
	string modifySubNodeName = modify(subNodeName);
	newNode.name_ = modifySubNodeName;
	newNode.pad_ = false;
	newNode.type_ = t;
	newNode.x = x;
	newNode.y = y;
	assert(firstThree == "VDD" || firstThree == "VSS");
	nodes.push_back(newNode);
	nameToIndex[modifySubNodeName] = nodes.size()-1;
	
	return true;
}

bool PowerGrid::addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value)
{
	string modifyNodeName1 = modify(nodeName1);
	string modifyNodeName2 = modify(nodeName2);

	assert(nameToIndex.find(modifyNodeName1) != nameToIndex.end());
	assert(nameToIndex.find(modifyNodeName2) != nameToIndex.end());
	
	int node1Index = nameToIndex[modifyNodeName1];
	int node2Index = nameToIndex[modifyNodeName2];

	Component newCom;
	newCom.type_ = Component::RES;
	newCom.value_ = value*1.1;
	newCom.vPos_ = node1Index;
	newCom.vNeg_ = node2Index;
	components.push_back(newCom);

	return true;
}

bool PowerGrid::addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value)
{	
	string modifyNodeName1 = modify(nodeName1);
	string modifyNodeName2 = modify(nodeName2);
	
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

	
	int node1Index = nameToIndex[modifyNodeName1];
	int node2Index = nameToIndex[modifyNodeName2];
	
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
	string modifyNodeName1 = modify(nodeName1);
	string modifyNodeName2 = modify(nodeName2);

	assert(nameToIndex.find(modifyNodeName1) != nameToIndex.end());
	assert(nameToIndex.find(modifyNodeName2) != nameToIndex.end());
	
	int node1Index = nameToIndex[modifyNodeName1];
	int node2Index = nameToIndex[modifyNodeName2];

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
	string modifyNodeName = modify(nodeName);
	assert(nameToIndex.find(modifyNodeName) != nameToIndex.end());
	int index = nameToIndex[modifyNodeName];
	nodes[index].pad_ = true;
	nodes[index].voltage_ = value;
	supplyVoltage = max(value , supplyVoltage);

	Component newCom;
	newCom.type_ = Component::VS;
	newCom.value_ = value;
	newCom.vPos_ = index;
	newCom.vNeg_ = nameToIndex["0"];
	components.push_back(newCom);
	return true;
}

int PowerGrid::addVDD(double vdd)
{
	int rightUpNode = 0;
	int leftDownNode = 0;
	int rightDownNode = 0;
	int leftUpNode = 0;
	for(unsigned i = 0 ; i < nodes.size() ; ++i)
	{
		Node node = nodes[i];
		if(node.type_ != Node::POWER)
			continue;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x >= nodes[rightUpNode].x && node.y >= nodes[rightUpNode].y)
			rightUpNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x <= nodes[leftDownNode].x && node.y <= nodes[leftDownNode].y)
			leftDownNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x >= nodes[rightDownNode].x && node.y <= nodes[rightDownNode].y)
			rightDownNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x <= nodes[leftUpNode].x && node.y >= nodes[leftUpNode].y)
			leftUpNode = i;
	}

	addIndependentVoltageSource(nodes[rightUpNode].name_ , vdd);
	gndNodes.push_back(rightUpNode);

	//addIndependentVoltageSource(nodes[leftDownNode].name_ , vdd);
	//gndNodes.push_back(leftDownNode);

	//addIndependentVoltageSource(nodes[rightDownNode].name_ , vdd);
	//vddNodes.push_back(rightDownNode);
	//
	//addIndependentVoltageSource(nodes[leftUpNode].name_ , vdd);
	//vddNodes.push_back(leftUpNode);
	return 0;
}

int PowerGrid::addGND(double gnd)
{
	int rightUpNode = 0;
	int leftDownNode = 0;
	int rightDownNode = 0;
	int leftUpNode = 0;
	for(unsigned i = 0 ; i < nodes.size() ; ++i)
	{
		Node node = nodes[i];
		if(node.type_ != Node::GROUND)
			continue;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x >= nodes[rightUpNode].x && node.y >= nodes[rightUpNode].y)
			rightUpNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x <= nodes[leftDownNode].x && node.y <= nodes[leftDownNode].y)
			leftDownNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x >= nodes[rightDownNode].x && node.y <= nodes[rightDownNode].y)
			rightDownNode = i;
		if(node.name_[4] <= '9' && node.name_[4] >='0' && node.x <= nodes[leftUpNode].x && node.y >= nodes[leftUpNode].y)
			leftUpNode = i;
	}

	addIndependentVoltageSource(nodes[rightUpNode].name_ , gnd);
	gndNodes.push_back(rightUpNode);

	//addIndependentVoltageSource(nodes[leftDownNode].name_ , gnd);
	//gndNodes.push_back(leftDownNode);

	//addIndependentVoltageSource(nodes[rightDownNode].name_ , gnd);
	//vddNodes.push_back(rightDownNode);
	//
	//addIndependentVoltageSource(nodes[leftUpNode].name_ , gnd);
	//vddNodes.push_back(leftUpNode);

	return leftDownNode;
}

