// **************************************************************************
// File       [ sdf_test.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ ]
// Date       [ 2011/07/19 created ]
// **************************************************************************

#include <cstdio>
#include <cstdlib>

#include "delay_info.h"
#include "delay_info_builder.h"

#include "circuit.h"
#include "circuit_builder.h"

#include "library.h"
#include "library_parser.h"

using namespace std;
using namespace IntfNs;


int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr, "**ERROR main(): please provide input verilog and sdf file\n");
        exit(0);
    }
	
	Library lib;
	LibraryParser libp(&lib);
	if(!libp.read(argv[1]))
	{
		cout << "**ERROR main(): LIB parser failed" <<endl;
		return 0;
	}
	

	//circuit builder declare
	CircuitBuilder cb;
	//setting
	cb.set(&lib);
	cb.set("CK");
	//read and build the circuit
	if(!cb.read(argv[2])){
		cout << "**ERROR main(): SDF parser failed" <<endl;
		return 0;
	}
	
	//get circuit
	Circuit cir = cb.getCircuit();

	//cir.print();
	

    DelayInfoBuilder dib;
	dib.set(&lib);
	dib.set(&cir);
    if (!dib.read(argv[3])) {
        fprintf(stderr, "**ERROR main(): SDF parser failed\n");
        exit(0);
    }
	cout << "done" <<endl;
    return 0;
}

