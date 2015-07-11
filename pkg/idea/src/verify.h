// **************************************************************************
// File       [ verify.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
#ifndef __IDEA_VERIFY__
#define __IDEA_VERIFY__
#include "core/src/circuit.h"
#include "core/src/wave.h"
#include "ir_core/src/power_grid.h"
#include "core/src/library.h"
#include "utility/src/spMaker.h"
#include <fstream>
class Idea;
class VerifyIdea
{
public:
	enum VERIFY_TYPE
	{
		HSPICE,
		NANO_SIM
	};
	VerifyIdea(){};
	~VerifyIdea(){};
	void set(Library *lib);
	void set(Idea *idea);
	void set(Circuit &cir);
	bool verify(Circuit &cir, Wave *waves, int patternID, const std::string &workspace);
	void pathVerify(const Transition *t , double pathDelay , double extraDelay);
	
	void singleVerify(Circuit &cir, Wave *waves, pgNs::PowerGrid &pg, const Transition *trans, VERIFY_TYPE type);

	std::vector<double> ideaPathDelay;
	std::vector<double> spicePathDelay;
	std::vector<double> ideaPathDelayIR;
	std::vector<double> spicePathDelayIR;


private:
	void pathVerify(Transition *t , double pathDelay , double extraDelay);
	void setPathMeasurement(bool ideal, SpMaker &sp , const Transition *trans, Circuit &cir);
	void printResult(SpMaker &sp , const Transition *trans, Circuit &cir, Wave *wave);
	std::string printNet(const std::string &nodeName, Wave &wave);
	Library *lib;
	Idea *idea;
	std::string spfFile;
	std::string dspfFile;
	Circuit *cir_;
};

#endif
