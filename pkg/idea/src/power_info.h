#ifndef __IDEA_POWER_INFO__
#define __IDEA_POWER_INFO__

#include "idea.h"
#include "core/src/circuit.h"
#include "core/src/wave.h"

class PowerInfo
{
public:
	int wsa(Idea *idea)
	{
		return idea->vls.size();
	}

	int maxHazard(Wave *wave , Circuit *cir)
	{
		int ret = 0;
		for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
		{
			if(wave[i].transition.size() > (unsigned)ret)
			{
				ret = wave[i].transition.size();
			}
		}
		return ret;
	}

	double avgHazard(Wave *wave , Circuit *cir)
	{
		double ret = 0;
		for(unsigned i = 0 ; i < cir->nets.size() ; ++i)
			ret += (double) wave[i].transition.size();
		
		ret /= (double) cir->nets.size();
		return ret;
	}

	int fftc(Wave *wave , Circuit *cir)
	{
		int fftc = 0;
		for(unsigned i = 0 ; i < cir->ff.size() ; ++i)
		{
			int cellID = cir->ff[i];
			Cell &cell = cir->cells[cellID];
			int netQID = cell.opt_net_id[0];
			fftc += wave[netQID].transition.size();
		}
		return fftc;
	}

	double maxIR(Idea *idea)
	{
		double swing(1000);
		for(unsigned i = 0 ; i < idea->vls.size() ; ++i)
			swing = std::min(swing , idea->vhs[i] - idea->vls[i]);
		return idea->pgs_->getPowerGrid()->getSupplyVoltage() - swing;
	}

	double avgIR(Idea *idea)
	{
		double vlsAvg(0), vhsAvg(0);
		for(unsigned i = 0 ; i < idea->vls.size() ; ++i)
		{
			vlsAvg += idea->vls[i];
			vhsAvg += idea->vhs[i];
		}
		vlsAvg/=(double)idea->vls.size();
		vhsAvg/=(double)idea->vhs.size();

		return idea->pgs_->getPowerGrid()->getSupplyVoltage() - ( vhsAvg - vlsAvg);
	}
};

#endif

