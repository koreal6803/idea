#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <iomanip>
#include "power_grid.h"
#include "power_grid_solver.h"

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

	
	vector<Node>  &pgNodes = powerGrid.getNodes();
	for(unsigned i = 0 ; i < pgNodes.size() ; i++)
		pgNodes[i].setVoltage(0);
	
	PowerGridSolver matrixSolver(&powerGrid, true);
	int cnt = 0;
	while(cnt++ < 100)
	{
		cout << "start iteration " << cnt <<endl;
		
		matrixSolver.solve();

		//print
		for(unsigned i = 0; i < pgNodes.size() ; i++)
		{
			if(pgNodes[i].isPad())
				cout << "PAD "; 
			else
				cout << "nod ";
			if(pgNodes[i].getType() == Node::POWER)
				cout << " power  network ";
			else
				cout << " ground network ";
			
			powerGrid.getSupplyVoltage();

			cout << setw(40) <<  pgNodes[i].getName() << setw(10) << setprecision(5) << pgNodes[i].getVoltage() <<endl;
		}
		cout << "end iteration... " << cnt << "/100 press any key to continue..." <<endl;
		getchar();
	}
	
	return 0;
}
