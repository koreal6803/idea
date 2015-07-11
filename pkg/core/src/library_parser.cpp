#include "library_parser.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <cassert>
using namespace std;

LibraryParser::LibraryParser(LibraryBasicBuilder *libraryBasicBuilder)
{
	builder_ = libraryBasicBuilder;
}

bool LibraryParser::read(const std::string &fileName , const set<string> &targetCellNames)
{
	std::ifstream fin(fileName.c_str());

	//find length of buffer
	fin.seekg (0, std::ios::end);
	int length = fin.tellg();
	fin.seekg (0, std::ios::beg);

	//creat a buffer
	char *buffer = new char[length+1];
	fin.read( buffer , length );
	buffer[length] = '\0';

	//get start and end of buffer
	char *begin = buffer;
	char *end = buffer + length;
	
	bool parseEverything(targetCellNames.size() == 0);
	
	//parse
	char *p = begin;
	LibNode *node = NULL;
	while(p != end)
	{
		blankAndSpace(p);
		Operation op = getInstruction(p);
		//cout << op << endl;
		switch(op)
		{
			case ADD_DATA:
			{
				string tag,dat;
				getData(p , tag , dat);
				node->addData(tag , dat);
				//if(node->getName() == "Z")
				//{
				//	cout << "nod: " << node->dataName_ <<endl;
				//	cout << "tag: " << tag << endl;
				//	cout << "  data: " << dat <<endl<<endl;
				//}
				break;
			}
			case ADD_DATA_ARRAY:
			{
				string tag;
				vector<string> datArray;
				getDataArray(p , tag , datArray);
				if(node->getName() == "Z")
				{
					//cout << "tag: " << tag <<endl;
					//for(int i = 0 ; i < datArray.size() ; i++)
					//	cout << "  data: " << datArray[i] <<endl<<endl;
				}
				node->addDataArray(tag , datArray);
				break;
			}
			case ADD_NODE:
			{
				string dataType , dataName;
				getNodeInform(p , dataType , dataName);
				//cout << "get node: " << dataType << "( " << dataName << " )" << endl;
				if(!parseEverything && dataType == "cell" && targetCellNames.find(dataName) == targetCellNames.end() )
				{
					//cout << "jump " << dataName <<endl;
					jumpCell(p);
					break;
				}
				if(node == NULL)
				{
					node = new LibNode(dataType , dataName , NULL);
					builder_->root_ = node;
				}
				else
				{
					//cout << "==========="<<endl;
					//cout << "add " << dataType << " " << dataName <<endl;
					//getchar();
					LibNode *childNode = new LibNode(dataType , dataName , node);
					node->addChild(childNode);
					node = childNode;
				}
				break;
			}
			case PARENT:
			{
				//cout << "==========" << endl;
				//cout << "jump" <<endl;
				//getchar();
				node = node->parent_;
				break;
			}
			case END:
				builder_->parseDone();
				return true;
			//default:
				//cout << "default" <<endl;
				//cout << op <<endl;
		}
		//getchar();
	}


	return p == end;
}

void LibraryParser::blankAndSpace(char *&p)
{
	//cout << *p << " " << *(p+1) << endl;
	while(*p == ' ' || *p == '\n' || *p == '\t' || (*p == '/' && (*(p+1) == '*' || *(p+1) == '/')) )
	{
		while(*p == ' ' || *p == '\n' || *p == '\t')
			p++;

		if(*p == '/' && *(p+1) == '*')
		{
			//cout << "find comment: " <<endl;
			p+=2;
			while(*p != '*' || *(p+1) != '/')
			{
				//cout << *p ;
				p++;
			}
			p+=2;
			//cout <<endl;
		}
		if(*p == '/' && *(p+1) == '/')
		{
			p+=2;
			while(*p != '\n')
				p++;
			p++;
		}
	}
}

LibraryParser::Operation LibraryParser::getInstruction(char *&p)
{
	if(*p == '\0')
		return END;
	if(*p == '}')
	{
		p++;
		return PARENT;
	}
	char *t = p;
	while(*t != '('&& *t != ':')
		t++;
	
	if(*t == ':')
		return ADD_DATA;
	
	while(*t != '{')
	{
		if(*t == '\n')
			return ADD_DATA_ARRAY;
		t++;
	}
	return ADD_NODE;
}
void jumpEndOfWord(char* &p , char n)
{
	//cout << "jump end of word: ";
	while(*p != ' ' && *p != n && *p != '\t' && *p != '\n')
	{
		//cout << *p;
		p++;
	}
	//cout << "end of jump" << endl;
}
void jumpEndOfWord(char* &p , char n , char k)
{
	//cout << "jump end of word" <<endl;
	while(*p != ' ' && *p != n && *p != k && *p != '\t' && *p != '\n')
		p++;
	//cout << "end of jump" <<endl;
}
void jumpSpaceAndN(char* &p , char n)
{	
	//cout << "jump space and n" <<endl;
	while(*p == ' '|| *p == n || *p == ':' || *p =='\t' || *p == '\n'|| *p == '\\')
		p++;
	//cout << "end of jump" <<endl;
}
void LibraryParser::getData(char* &p , string &tag , string &data)
{
	char *e = p;
	jumpEndOfWord(e , ':');
	tag.assign(p , e);
	p = e;
	jumpSpaceAndN(p , ':');
	e = p;
	if(*e == '"')
	{
		e++;
		p++;
		while(*e != '"')
			e++;
	
	}else
		jumpEndOfWord(e , ';');

	data.assign(p , e);
	p = e;
	if(*e == '"')
		p++;
	jumpSpaceAndN(p , ';');
}

void LibraryParser::getDataArray(char* &p , string &tag , vector<string> &dataArray )
{
	char *e = p;
	jumpEndOfWord(e , '(');
	tag.assign(p , e);
	p = e;
	jumpSpaceAndN(p , '(');
	while(*p != ')')
	{
		e = p;
		if(*e == '"')
		{
			e++;
			p++;
			while(*e !='"')
				e++;
		}else
			jumpEndOfWord(e , ',' , ')');
		
		string dat;
		dat.assign(p,e);
		dataArray.push_back(dat);
		p = e;
		if(*e == '"')
			p++;
		jumpSpaceAndN(p , ',');
	}
	while(*p!=';' && *p!='\n')
		p++;
	p++;
}

void LibraryParser::getNodeInform(char* &p , string &dataType , string &dataName)
{
	char *e = p;
	jumpEndOfWord(e , '(');
	dataType.assign(p , e);
	p = e;
	jumpSpaceAndN(p , '(');
	e = p;
	jumpEndOfWord(e , ')');
	dataName.assign(p , e);
	p = e;
	while(*p!='{')
		p++;
	p++;
}

void LibraryParser::jumpCell(char * &p)
{
	int branch(1);
	while(branch)
	{
		if(*p == '{')
			branch++;
		else if(*p == '}')
			branch--;
		p++;
	}
}


