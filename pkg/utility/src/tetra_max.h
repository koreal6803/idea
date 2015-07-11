#ifndef __UTILITY_FAULT_SIM__
#define __UTILITY_FAULT_SIM__

#include <string>

class TetraMax
{
public:
	TetraMax();

	// workspace
	void setWorkSpace(const std::string &workSpace);

	// general settings
	void setVerilog(const std::string &verilog);
	void setLibrary(const std::string &library);
	void setCircuit(const std::string &circuit);
	void setFaults(const std::string &faultList);

	// fault simulation settings
	void setPatterns(const std::string &wsl);
	void setOutputFaultList(const std::string &faultList);
	void fsim(const std::string &fName);

	// pattern generation settings
	void setOutputWsl(const std::string &wsl);
	void setOutputStil(const std::string &stil);
	void atpg(const std::string &fName);

private:
	std::string workSpace_;
	std::string verilog_;
	std::string library_;
	std::string circuit_;
	std::string faultList_;
	std::string wsl_;
	std::string optFaultList_;
	std::string optWsl_;
	std::string optStil_;

};

#endif
