// **************************************************************************
// File       [ vlog_test.cpp ]
// Author     [ littleshamoo ]
// Synopsis   [ test verilog parser ]
// Date       [ 2010/12/29 created ]
// **************************************************************************

#include <cstdio>
#include <cstdlib>

#include "circuit.h"
#include "circuit_builder.h"

#include "library.h"
#include "library_parser.h"

using namespace std;


int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "**ERROR main(): please provide input library and verilog\n");
        exit(0);
    }
	
	Library lib;
	LibraryParser libp(&lib);

	libp.read(argv[1]);
	cout << "parse lib done" <<endl;
	CircuitBuilder cb;
	cb.set(&lib);
	cb.set("CK");
	// set flip flop functional pins
	cb.set("Q","QN","D","SI","SE","CK");


    if (!cb.read(argv[2])) {
        fprintf(stderr, "**ERROR main(): verilog parser failed\n");
        exit(0);
    }
	
	Circuit cir = cb.getCircuit();
	cir.print();

    return 0;
}

