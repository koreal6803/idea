#ifndef BUILDER_LIBRARY_BUILDER_H
#define BUILDER_LIBRARY_BUILDER_H

#include "ir_interface/src/libraryBasicBuilder.h"
#include <map>
#include "math.h"
#include "cassert"
enum LibReturnType
{
	PROPAGATION_DELAY = 0,
	TRANSITION_TIME = 2,
	INTERNAL_POWER = 4
};

struct LookUpTableInfo
{
	LibReturnType 	returnType;//ex: PROPATATION_DELAY
	int 			cellIndex;
	int 			pinIndex;
	int 			relativePinIndex;
	int 			faninSignal;
	bool 			toggleRise;
	double 			capacitance;
	double 			inputTransition;//ex: 0.004ns
};

struct LookUpTableInfoString
{
	LibReturnType	returnType;
	std::string 	cellType;
	std::string 	pinName;
	std::string 	relativePinName;
	std::vector<std::string>fanInSignal;
	bool 			toggleRise;
	double 			capacitance;
	double 			inputTransition;
};

struct Lut;
struct FaninSigToLut;

class Library:public LibraryBasicBuilder
{
	friend class LibraryParser;
	public:
		Library();
		bool readDelay(const std::string &fName);

		//put the data information(by index) into "info" then get the value in the look up table
		double get(LookUpTableInfo &info);

		//put the data information(by name ) into "info" then get the value in the look up table
		double get(LookUpTableInfoString &info);
		
		//get capacitance by cell index, fanOut pin index and fanIn pin index
		double getCapacitance(LookUpTableInfo &info);

		//get function by cellId and fanOut pin index
		const std::string &getFunction(int cellTypeIndex , int pinIndex);
		
		// get cell & pin index
		int getCellIndex(const std::string &cellType);
		int getFanoutIndex(int cellTypeIndex , const std::string &pinName);
		int getFaninIndex(int cellTypeIndex , const std::string &relativePinName);
		std::string getFanoutName(int cellTypeIndex, int fanoutIndex);
		std::string getFaninName(int cellTypeIndex, int faninIndex);
		bool isFanout(int cellIndex , const std::string &pinName);
		bool isFanin(int cellIndex , const std::string &pinName);
		int getFanoutSize(int cellIndex);
		int getFaninSize(int cellIndex);
	private:
		//Operation after parse done!
		void parseDone();
		void buildTemplate();
		void buildIndexCapaFunc();
		void buildLookUpTable();
		void lutEncode();
		
		//for encoding
		int maxFanoutFanin;
		
		std::vector<Lut>  lookUpTables_;
		std::vector<std::vector<FaninSigToLut> > fanInSigToLut_;

		void getPerciseIndex(double capa , double time , int templateIndex , double &x , double &y);

		void addSig(int cellIndex , const std::string &signal , int lutIndex , int signalToLutIndex,int returnType);
		int getLutIndex(const std::string &signal);
	
		//template
		std::vector<std::vector<double> > template_;
		std::map<std::string , int> templateIndex_;

		//index varible
		std::map<std::string , int> cellIndex_;
		std::vector<std::map<std::string , int> >pinIndex_;
		std::vector<std::map<std::string , int> >relativePinIndex_;

		//capacitance
		std::vector<std::vector<double> > riseCapacitance_;
		std::vector<std::vector<double> > fallCapacitance_;

		//string
		std::vector<std::vector<std::string> > function_;

		//delay or transition
		bool readDelay_;
		std::vector<std::vector<std::vector<double> > > tables_;
		std::vector<std::vector<int > > encodes_;
};


struct Lut
{
	double index[7][7];
	int templateIndex;
	LibNode* node;
};

struct FaninSigToLut
{
	std::vector<int>sigToLut;
	int lutAmount;
};

#endif
