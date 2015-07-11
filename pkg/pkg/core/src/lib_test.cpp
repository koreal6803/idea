#include <iostream>
#include "library.h"
#include "library_parser.h"
using namespace std;

int main(int argc , char ** argv)
{
	set<string> cellNames;
	cellNames.insert("NAND2_X2");
	cellNames.insert("AND2_X2");
	cellNames.insert("OR2_X2");
	cellNames.insert("NOR2_X2");
	if(argc < 2)
	{
		cout << "please input the library file" << endl;
		return 0;
	}
	Library lb;
	LibraryParser lp(&lb);
	lp.read(argv[1], cellNames);

	int nandCellIndex = lb.getCellIndex("AND2_X2");
	cout << "AND2_X2" << nandCellIndex <<endl;
	cout << "ZN: " << lb.getFanOutIndex(nandCellIndex, "ZN") << "\t OUTPUT: " << lb.isFanOut(nandCellIndex, "ZN") << "\t INPUT: " << lb.isFanIn(nandCellIndex, "ZN")<< endl;
	cout << "A1: " << lb.getFanInIndex(nandCellIndex, "A1") <<  "\t OUTPUT: " << lb.isFanOut(nandCellIndex, "A1") << "\t INPUT: " << lb.isFanIn(nandCellIndex, "A1")<< endl;
	cout << "A2: " << lb.getFanInIndex(nandCellIndex, "A2") <<  "\t OUTPUT: " << lb.isFanOut(nandCellIndex, "A2") << "\t INPUT: " << lb.isFanIn(nandCellIndex, "A2")<< endl;

	LookUpTableInfo info;
	info.cellIndex = nandCellIndex;
	info.relativePinIndex = 0; // A1

	info.toggleRise = true;
	cout << "A1_CAR: " << lb.getCapacitance(info) <<endl;
	info.toggleRise = false;
	cout << "A1_CAF: " << lb.getCapacitance(info) <<endl;
	
	info.relativePinIndex = 1; // A2
	info.toggleRise = true;
	cout << "A2_CAR: " << lb.getCapacitance(info) <<endl;
	info.toggleRise = false;
	cout << "A2_CAF: " << lb.getCapacitance(info) <<endl;

	// transition from A1 to ZN iptTransitionTime = 0.0075 optCapacitance = 0.0004
	info.pinIndex = 0; // ZN
	info.relativePinIndex = 0; // A1
	info.capacitance = 0.0004;
	info.inputTransition = 0.0075;
	info.faninSignal = 0;
	
	info.returnType = TRANSITION_TIME;
	cout << "transition time: " << lb.get(info) <<endl;

};
