#ifndef INTERFACE_LIBRARYPARSER
#define INTERFACE_LIBRARYPARSER

#include "ir_interface/src/libraryBasicBuilder.h"
#include <set>
class LibraryParser
{
	public:
		LibraryParser(LibraryBasicBuilder *libraryBasicBuilder);
		bool read(const std::string &fileName, const std::set<std::string> &targetCellNames = std::set<std::string>());
	private:
		enum Operation
		{
			ADD_DATA, ADD_DATA_ARRAY, ADD_NODE , PARENT , END
		};
		void blankAndSpace(char *&p);
		Operation getInstruction(char *&p);
		void getData(char* &p , std::string &tag , std::string &dat);
		void getDataArray(char* &p , std::string &tag , std::vector<std::string> &dataArray);
		void getNodeInform(char* &p , std::string &dataType , std::string &dataName);
		void jumpCell(char * &p);
		LibraryBasicBuilder *builder_;
};

#endif
