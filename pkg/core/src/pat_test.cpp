#include <iostream>
using namespace std;

#include "circuit.h"
#include "circuit_builder.h"
#include "cell_mgr.h"
#include "pattern_set.h"
#include "library.h"
#include "library_parser.h"
int main(int argc, char** argv)
{
	if(argc < 3)
	{
		cout << "please input the circuit.v and circuit.pat" <<endl;
		return 0;
	}
	Library lib;
	LibraryParser libp(&lib);
	if(!libp.read(argv[1]))
	{
		cout << "**ERROR main(): LIB parser failed" <<endl;
		return 0;
	}
	
	CircuitBuilder cb;
	
	cb.set(&lib);
	cb.set("CK");

	if(!cb.read(argv[2]))
	{
		cout << "parse verilog error!" <<endl;
		return 0;
	}
	Circuit cir = cb.getCircuit();

	PatternSet ps;
	
	ps.set(&cir);
	
	if(!ps.setFile(argv[3]))
	{
		cout << "parse pattern file error!" <<endl;
		return 0;
	}
	while(ps.readNextPattern())
	{
		const Pattern p = ps.getPattern();
		cout << "pattern: " << p.ppi << " " << p.ppo <<endl;
	}

	return 0;
}
