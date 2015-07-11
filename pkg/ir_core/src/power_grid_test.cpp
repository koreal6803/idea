#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include "power_grid.h"

using namespace std;
using namespace IntfNs;
using namespace pgNs;
int main(int argc , char ** argv)
{
	if(argc < 2)
	{
		cout << "*ERROR please give me a dspf file" <<endl;
		return 0;
	}
	
	ifstream fin(argv[1]);
	if(!fin)
	{
		cout << "*ERROR can't find file: " << argv[1] <<endl;
		return 0;
	}
	
	PowerGrid powerGrid;
	powerGrid.read(argv[1]);
	
	
	const vector<Component> &components = powerGrid.getComponents();
	for(unsigned i = 0 ; i < components.size() ; i++)
		cout << "components type: " << components[i].getType() << " vpos: " << components[i].getVPos() << " vneg: " << components[i].getVNeg() << " val: " << components[i].getValue() <<endl;
	cout <<endl << endl;
	cout << "read power grid success" <<endl;
	return 0;
}
