#ifndef __DEF_PARSER__
#define __DEF_PARSER__
#include <vector>
#include <string>
#include <map>
class DefParser
{
public:
	bool read(const std::string &fName);
	int chipx;
	int chipy;
	int unit;
	std::map<std::string , std::vector<int> > cellLocate;

};

#endif
