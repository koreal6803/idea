#ifndef __IR_NET_CAP_EXT__
#define __IR_NET_CAP_EXT__

#include <vector>
#include <map>
#include <string>
#include "ir_interface/src/spf_file.h"
#include "circuit.h"

class NetCapExtracter: public IntfNs::SpfFile
{
public:
	
	NetCapExtracter(Circuit *cir):cir_(cir){};
	virtual ~NetCapExtracter();

private:
	
	Circuit *cir_;
	//building power grid functions
	bool addNet(const std::string& netName , double capacitance)
	{
		int netID = cir_->getNetID(netName);
		cir_->nets[netID].totalRiseCap+=capacitance;
		cir_->nets[netID].totalFallCap+=capacitance;
	}
	bool addPin(const std::string& pinName , char ioType , double pinCapacitance ,  double x , double y){};
	bool addInstancePin(const std::string& instancePinName , const std::string& instanceName, const std::string& pinName , char ioType , double capacitance , double x , double y){}
	bool addSubNode(const std::string& subNodeName , double x , double y){}
	bool addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value){}
	bool addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value){}
	bool addInductance(const std::string& lName , const std::string& nodeName1 , const std::string& nodeName2 , double value){}
	bool addIndependentVoltageSource(const std::string& nodeName , double value){]


	//power grid nodes
	std::vector<Node> 			nodes;
	
	//pgNode name to index
	std::map<std::string , int> nameToIndex;
	
	//Components
	std::vector<Component> 		components;

	//VDD
	double 						supplyVoltage;
};

#endif
