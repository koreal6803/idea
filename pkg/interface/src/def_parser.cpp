#include "def_parser.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

bool DefParser::read(const string &fName)
{
	ifstream fin(fName.c_str());

/*
//UNITS DISTANCE MICRONS 2000 ;
//DIEAREA ( 0 0 ) ( 335880 331360 ) ;
//COMPONENTS 14 ;
//- CK__L1_I0 BUF_X8 + SOURCE TIMING + FIXED ( 170620 162960 ) N + WEIGHT 1
//- U11 INV_X1 + PLACED ( 170240 165760 ) FS
*/
	if(!fin)
		return false;
	string line;
	while(fin >> line)
	{
		if(line == "MICRONS")
			fin >> unit;
		else if(line == "DIEAREA")
			fin >> line >> line >> line >> line >> line>> chipx >> chipy;
		else if(line == "COMPONENTS")
		{
			int comSize;
			fin >> comSize;

			int cnt = 0;;
			string cellName;
			while(fin >> line)
			{
				
				if(line[0] == '(')
				{
					vector<int> vec(2,-1);
					fin >> vec[0] >> vec[1];
					cellLocate[cellName] = vec;
					cnt++;
				}
				if(line[0] == '-')
				{
					fin >> cellName;
				}
				if(cnt == comSize)
					return true;
			}
			return false;
		}
	}
	return false;
}
