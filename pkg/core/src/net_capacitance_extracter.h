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
	~NetCapExtracter(){};

private:
	
	Circuit *cir_;
	//building power grid functions
	bool addNet(const std::string& netName , double capacitance)
	{
		int netID = cir_->getNetID(netName);
		if(netID == -1 && (netName == "VDD" || netName == "VSS"))
			return true;
		else if(netID == -1)
		{
			std::cout << "**WARRN NetCapExtracter::addNet: can't find net: " << netName << std::endl;
			return true;
		}
		
		// add the capacitance parsed and change the capacitance unit to PF
		cir_->nets[netID].totalRiseCap+=capacitance*1e12;
		cir_->nets[netID].totalFallCap+=capacitance*1e12;
		return true;
	}
	bool addPin(const std::string& pinName , char ioType , double pinCapacitance ,  double x , double y){return true;};
	bool addInstancePin(const std::string& instancePinName , const std::string& instanceName, const std::string& pinName , char ioType , double capacitance , double x , double y){return true;}
	bool addSubNode(const std::string& subNodeName , double x , double y){return true;}
	bool addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value){return true;}
	bool addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value){return true;}
	bool addInductance(const std::string& lName , const std::string& nodeName1 , const std::string& nodeName2 , double value){return true;}
	bool addIndependentVoltageSource(const std::string& nodeName , double value){return true;}


};

#endif
