#include <iostream>
#include <string>
#include "wave_view.h"
#include <cstdlib>
using namespace std;

int main(int argc, char ** argv)
{
	if(argc < 2)
	{
		cout << "please input the idea log file" << endl;
		return 0;
	}
	WaveView  wv(argv[1]);
	while(1)
	{
		cout << "wave-view >> ";

		string in;
		cin >> in;
		
		if(wv.writeVoltage(in,"log/wave.html")){
			cout << "success!" << endl;
			system("mv log/wave.html ~/public_html/mywork/view/");
		}
		else
			cout << "cant find the node: " << in << endl;
	}
}
