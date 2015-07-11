
// **************************************************************************
// File       [ cell_extracter.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/11 created ]
// **************************************************************************

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
using namespace std;


int main(int argc , char** argv)
{
    if(argc != 2){
		cout << "usage: ./bin lib_file" <<endl;
		return 0;
	}
	
	cout << "#include <vector>" <<endl;
	cout << "#include <map>" <<endl;
	cout << "#include <string>" <<endl;
	ifstream fin(argv[1]);
	
	vector<string> cells;
	vector<vector<int> > pinID;
	vector<vector<string> > pinName;
	vector<vector<bool> > isOutput;

	string in;
	while(fin >> in)
	{
		if(in == "Module"){
			
			fin >> in >> in;
			cells.push_back(in);
			pinName.push_back(vector<string>());
			isOutput.push_back(vector<bool>());
		}
		else if(in == "pin"){
			fin >> in;
			in.erase(in.begin());
			in.erase(in.end()-1);
			pinName.back().push_back(in);

			fin >> in >> in;
			assert(in == "direction");
			fin >> in >> in;

			bool output = (in == "output;")? true : false;
			isOutput.back().push_back(output);
		}
	}

	cout << "class CellMgr" <<endl;
	cout << "{" <<endl;
	cout << "    std::map<std::string,int> cellID;" <<endl;
	cout << "    std::vector<std::map<std::string,int> > pinID;" <<endl;
	cout << "    std::vector<std::map<std::string,int> > isOutput;" <<endl;
	cout << "    CellMgr(){" <<endl;
	
	for(unsigned i = 0 ; i < cells.size() ; i++){
	    cout << "        cellID[\"" << cells[i] << "\"] = " << i << ";"<<endl;
		cout << "          pinID.push_back(std::map<std::string,int>());" <<endl;
		cout << "          isOutput.push_back(std::map<std::string,int>());" <<endl;
		for(unsigned j = 0 ; j < pinName[i].size() ; j++){
			cout << "          pinID.back()[\""<<pinName[i][j]<<"\"] = " << j << ";" << endl;
			cout << "          isOutput.back()[\""<<pinName[i][j]<<"\"] = " << isOutput[i][j] << ";" << endl;
		}
		cout <<endl;
	}
	cout << "    }" <<endl;
	cout << "};" <<endl;

	
	return 0;
}
