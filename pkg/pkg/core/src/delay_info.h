

// **************************************************************************
// File       [ delay_info.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#ifndef __CORE_DELAY_INFO_H__
#define __CORE_DELAY_INFO_H__

#include <vector>
class DelayInfo
{
friend class DelayInfoBuilder;
	std::vector<std::vector<double> > riseGateDelay; // [cellID] [optPinID * maxIptPinID + iptPinID]
	std::vector<std::vector<double> > fallGateDelay; 
	std::vector<int> iptSize;

	std::vector<std::vector<double> > riseNetDelay; // [cellID] [iptPinID]
	std::vector<std::vector<double> > fallNetDelay;

public:
	double getGateDelay(int cellID , int optID , int iptID , bool transition_rise)
	{
		if(transition_rise)
			return riseGateDelay[cellID][optID*iptSize[cellID]+ iptID];
		return fallGateDelay[cellID][optID*iptSize[cellID]+ iptID];
	}
	double getNetDelay(int cellID, int iptPinID, bool transition_rise)
	{
		if(transition_rise)
			return riseNetDelay[cellID][iptPinID];
		return fallNetDelay[cellID][iptPinID];
	}
};

#endif
