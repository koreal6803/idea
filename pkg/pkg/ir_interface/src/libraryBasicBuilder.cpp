#include "libraryBasicBuilder.h"

#include <cassert>
using namespace std;

LibraryBasicBuilder::LibraryBasicBuilder()
{root_ = NULL;}

LibraryBasicBuilder::~LibraryBasicBuilder()
{
	delete root_;
}

const LibNode* const LibraryBasicBuilder::getRoot()
{
	return root_;
}

LibNode::LibNode(const string &dataType , const string &dataName , LibNode *parent)
{
	dataType_ = dataType;
	dataName_ = dataName;
	parent_ = parent;
}

LibNode::~LibNode()
{
	vector<int> test;
	for(vector<LibNode*>::iterator it = childs_.begin() ; it != childs_.end() ; it++)
		delete *it; 
}

void LibNode::addData(const std::string &tag , const std::string &dat)
{
	datas_[tag] = dat;
}

void LibNode::addDataArray(const string &tag , const vector<string> &dat)
{
	dataArrays_[tag] = dat;
}

bool LibNode::isArray(const string &tag)const
{
	map<string,vector<string> >::const_iterator finder = dataArrays_.find(tag);
	return finder != dataArrays_.end();
}

bool LibNode::existTag(const string &tag)const
{
	map<string,vector<string> >::const_iterator finder = dataArrays_.find(tag);
	if(finder != dataArrays_.end())
		return true;
	
	map<string,string>::const_iterator finder2 = datas_.find(tag);
	if(finder2 != datas_.end())
		return true;
	return false;
}

const string &LibNode::getData(const string &tag)const
{
	assert(existTag(tag));

	assert(!isArray(tag));

	map<string,string>::const_iterator finder = datas_.find(tag);
	return finder->second;
}

const vector<string> &LibNode::getDataArray(const string &tag)const
{
	assert(existTag(tag));

	assert(isArray(tag));

	map<string,vector<string> >::const_iterator finder = dataArrays_.find(tag);
	return finder->second;
}

const LibNode* const LibNode::getParent()const
{
	return parent_;
}

const vector<LibNode*> LibNode::getChilds()const
{
	return childs_;
}

void LibNode::addChild(LibNode *newNode)
{
	childs_.push_back(newNode);
}
