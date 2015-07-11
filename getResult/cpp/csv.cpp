#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

int main(int argc , char ** argv)
{
	// usage ./binary file -p max,delay -all circuit
	if(argc < 2 && argc%2 == 0)
	{
		cout << "please input the log file" << endl;
		return 0;
	}


	ifstream fin(argv[1]);
	if(!fin)
	{
		cout << "can't find file: " << argv[1] << endl;
		return 0;
	}
	
	vector<bool> pat;
	for(unsigned i = 2 ; i < argc ; i+=2)
	{
		string flag(argv[i]);
		if(flag == "-pat")
		{
			pat.push_back(true);
		}
		else if(flag == "-all")
		{
			pat.push_back(false);
		}
		else
		{
			cout << "wrong argument format" << endl;
			return 0;
		}
	}

	vector<vector<string> > keywords;
	for(unsigned i = 3 ; i < argc ; i+=2)
	{
		vector<string> keys;
		string keyword(argv[i]);
		for(unsigned j = 0 ; j < keyword.size() ; j++)
			if(keyword[j] == ',')
				keyword[j] = ' ';
		stringstream ss(keyword);
		string key;
		cout << "type id=" << keywords.size() << ", ";
		while(ss >> key)
		{
			cout << key << ", ";
			keys.push_back(key);
		}
		cout << endl;

		keywords.push_back(keys);
	}
	cout << endl;


	string line;
	int patCount(-1);
	vector<vector<double> > table(keywords.size() , vector<double>());

	while(getline(fin , line))
	{
		if(line.find("Pattern 0") != string::npos)
			patCount++;
		if(line.find("Pattern") != string::npos && patCount != -1)
			patCount++;

		for(unsigned i = 0 ; i < keywords.size() ; i++)
		{
			if(pat[i] && patCount == -1)
				continue;
			bool found = true;
			for(unsigned j = 0 ; j < keywords[i].size() ; j++)
			{
				if(line.find(keywords[i][j]) == string::npos)
				{
					found = false;
					break;
				}
			}
			if(found)
			{
				unsigned spos = line.find_last_of('-');
				line.erase(line.begin() , line.begin() + spos+1);
				stringstream ss(line);
				double value;
				ss >> value;
				table[i].push_back(value);
			}
		}
	}
	for(unsigned i = 0 ; i < table[0].size() ; ++i)
	{
		for(unsigned j =  0 ; j < table.size() ; ++j)
		{
			if(j == table.size() -1)
				cout << table[j][i] << endl;
			else
				cout << table[j][i] << ", ";
		}
	}
	return 0;
}
