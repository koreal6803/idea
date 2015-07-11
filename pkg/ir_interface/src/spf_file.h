#ifndef __IR_INTERFACE_SPF_FILE_H__
#define __IR_INTERFACE_SPF_FILE_H__

#include <string>


namespace IntfNs {

class SpfFile;
typedef SpfFile DspfFile;

class SpfFile
{
public:
	SpfFile();
	virtual ~SpfFile();

	bool read(const char* const fileName);
	
	virtual bool addNet( const std::string& netName , double capacitance);
	virtual bool addPin(const std::string& pinName , char ioType , double pinCapacitance ,  double x , double y);
	virtual bool addInstancePin(const std::string& instancePinName , const std::string& instanceName, const std::string& pinName , char ioType , double capacitance , double x , double y);
	virtual bool addSubNode(const std::string& subNodeName , double x , double y);
	virtual bool addResistor(const std::string& resistorName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	virtual bool addCapacitance(const std::string& capacitanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	virtual bool addInductance(const std::string &inductanceName , const std::string& nodeName1 , const std::string& nodeName2 , double value);
	virtual bool addIndependentVoltageSource(const std::string& nodeName , double value);
};


};

#endif
