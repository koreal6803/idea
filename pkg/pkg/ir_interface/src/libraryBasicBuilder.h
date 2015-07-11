#ifndef INTERFACE_LIBRARY_BASIC_BUILDER_H
#define INTERFACE_LIBRARY_BASIC_BUILDER_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
class LibNode;

class LibraryBasicBuilder
{
	friend class LibraryParser;
	public:
		LibraryBasicBuilder();
		~LibraryBasicBuilder();
		const LibNode* const getRoot();
	protected:
		virtual void parseDone(){std::cout << "library basic builder: parser done!" << std::endl;};
		LibNode *root_;
};

class LibNode
{
	friend class LibraryParser;
	public:
		LibNode(const std::string &dataType , const std::string &dataName , LibNode *parent);
		~LibNode();

		void addData(const std::string &tag , const std::string &dat);
		void addDataArray(const std::string &tag , const std::vector<std::string> &dat);
		bool isArray(const std::string &tag)const;
		bool existTag(const std::string &tag)const;

		const std::string &getType()const{return dataType_;};
		const std::string &getName()const{return dataName_;};
		const std::string &getData(const std::string &tag)const;
		const std::vector<std::string> &getDataArray(const std::string &tag)const;
		const LibNode* const getParent()const;
		const std::vector<LibNode*>  getChilds()const;
		void addChild(LibNode *newNode);

	private:
		
		LibNode *parent_;
		std::vector<LibNode*> childs_;
		
		std::string dataType_;
		std::string dataName_;

		std::map<std::string , std::string> datas_;
		std::map<std::string , std::vector<std::string> > dataArrays_;
		
};


#endif
