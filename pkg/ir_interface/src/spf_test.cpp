#include <iostream>
#include <fstream>
#include "spf_file.h"

using namespace std;
using namespace IntfNs;

int main(int argc , char ** argv)
{
	DspfFile spfFile;

	ifstream fin(argv[1]);
	if(!fin)
	{
		cout << "*ERROR can't open spf file: " << argv[1] <<endl;
		return 0;
	}
	if(!spfFile.read(argv[1]))
	{
		cout << "*ERROR something wrong with parser" << endl;
		return 0;
	}
	cout << "success" <<endl;
	
	return 0;
}
