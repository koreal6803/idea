#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc , char **argv)
{
	if(argc < 2)
		return 0;
	
	ifstream fin(argv[1]);
	
	string line;
	bool found = false;
	while(getline(fin, line))
	{
		if(!found && line.find("ScanCells")!=string::npos)
		{
			string line1 , line2;
			getline(fin , line1);
			getline(fin , line2);
			cout << line << " " << line1 << " " << line2 << endl;
			found = true;
		}
		else
			cout << line << endl;
	}
}
