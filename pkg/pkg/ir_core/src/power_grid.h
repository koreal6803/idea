#ifndef __IR_CORE_POWER_GRID__
#define __IR_CORE_POWER_GRID__

#include <vector>
#include <map>
#include <string>
#include "ir_interface/src/spf_file.h"
#include "power_grid_node.h"
#include "component.h"
namespace pgNs
{


class PowerGrid: public IntfNs::SpfFile
{
public:
	
	PowerGrid();
	virtual ~PowerGrid();

	std::vector<Node>&				getNodes()			{return nodes;}
	std::vector<Component> 	getComponents()		{return components;}
	double 							getSupplyVoltage()	{return supplyVoltage;}
	int                             getPowerNode(const std::string &cellName)
    {
        std::string name = cellName + ".VDD";
        if(nameToIndex.find(name)== nameToIndex.end())
            return -1;
        else
            return nameToIndex[name];
    }

	int                             getGroundNode(const std::string &cellName)
    {
        std::string name = cellName + ".VSS";
        if(nameToIndex.find(name)== nameToIndex.end())
            return -1;
        else
            return nameToIndex[name];
    }

	void addVDD(double vdd);
	void addGND(double gnd);

private:

	//building power grid functions
	bool addNet(const std::string& netName , double capacitance);
	bool addPin(const std::string& pinName , char ioType , double pinCapacitance ,  double x , double y);
	bool addInstancePin(const std::string& instancePinName , const std::string& instanceName, const std::string& pinName , char ioType , double capacitance , double x , double y);
	bool addSubNode(const std::string& subNodeName , double x , double y);
	bool addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	bool addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	bool addInductance(const std::string& lName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	bool addIndependentVoltageSource(const std::string& nodeName , double value);
	

	//power grid nodes
	std::vector<Node> 			nodes;
	
	//pgNode name to index
	std::map<std::string , int> nameToIndex;
	
	//Components
	std::vector<Component> 		components;

	//VDD
	double 						supplyVoltage;


};

};
#endif
