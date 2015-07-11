#ifndef __UTILITY_WAVE_VIEW__
#define __UTILITY_WAVE_VIEW__

#include <vector>
#include <map>
#include <string>

class WaveView
{
public:
	
	WaveView(const char *fName);
	bool writeVoltage(const std::string nodeName , const std::string &optFile);

private:
	double timeStep;
	std::map<std::string,int> nodeIDs;
	std::vector<std::vector<double> > voltages;
	std::vector<std::vector<double> > currents;
};

#endif
