#include "library.h"

#include <cassert>
#include <sstream>
#include <math.h>
#include <queue>
#include "stdio.h"
#include "stdlib.h"
using namespace std;

Library::Library()
{
	maxFanOutFanIn = 0;
}

void Library::parseDone()
{
	assert(root_ != 0);
	//template builder
	//cout << "build template" <<endl;
	buildTemplate();
	//cout << "build index capa & index" <<endl;
	buildIndexCapaFunc();
	//cout << "build look up table" <<endl;
	buildLookUpTable();
	
	//cout << "encode" <<endl;
	//encode precount
	for(unsigned i = 0 ; i < cellIndex_.size() ; i++)
		if(pinIndex_[i].size()*relativePinIndex_[i].size() > (unsigned)maxFanOutFanIn)
			maxFanOutFanIn = pinIndex_[i].size()*relativePinIndex_[i].size();
	
	
	//declare fanInSigToLut_ enough size
	vector<FanInSigToLut> empty;
	empty.assign( cellIndex_.size() * maxFanOutFanIn , FanInSigToLut());
	fanInSigToLut_.assign(6 , empty);
	
	//cout << "build lut" <<endl;
	lutEncode();
	//cout << "done" <<endl;
}
void Library::buildTemplate()
{
	const vector<LibNode*> &childs = root_->getChilds();
	for(vector<LibNode*>::const_iterator n = childs.begin() ; n != childs.end() ; n++)
	{
		LibNode *node = *n;
		//cout << "type:\t" << node->getType() << "\t name:\t" << node->getName() <<endl;
		if(node->getType() == "power_lut_template" || node->getType() == "lu_table_template")
		{
			//cout << "find template " <<endl;
			templateIndex_[node->getName()] = template_.size();
			//cout << node->getName() << " " << templateIndex_[node->getName()] <<endl;
			if(node->existTag("index_1"))
			{
				vector<double> index1(7);
				stringstream s;
				s << node->getDataArray("index_1")[0] << ',';
				char c;
				for(unsigned i = 0 ;i < 7 ; i++)
					s >> index1[i] >> c;
				template_.push_back(index1);
			}
			if(node->existTag("index_2"))
			{
				vector<double> index2(7);
				stringstream s;
				s << node->getDataArray("index_2")[0] << ',';
				char c;
				for(unsigned i = 0 ;i < 7 ; i++)
					s >> index2[i] >> c;
				template_.push_back(index2);
			}
		}
	}
}
void Library::buildIndexCapaFunc()
{
	int cellIndex(0);
	//index builder , capacitance builder , function builder
	const vector<LibNode*> &childs = root_->getChilds();
	for(vector<LibNode*>::const_iterator n = childs.begin() ; n != childs.end() ; n++)
	{
		LibNode *node = *n;
		if(node->getType() == "cell")
		{
			//index builder
			int pinIndex(0);
			int relativePinIndex(0);
			map<string , int>pinIndexMap;
			map<string , int>relativePinIndexMap;
			
			//capacitance builder
			vector<double>riseCapa;
			vector<double>fallCapa;

			//function builder
			vector<string>optFunction;
			
			const vector<LibNode*> childs = node->getChilds();
			for(vector<LibNode*>::const_iterator p = childs.begin() ; p != childs.end() ; p++)
			{
				LibNode *pinNode = *p;
				//cout << "\t" <<pinNode->getType() << " " << pinNode->getName() <<endl;
				if(pinNode->getType() == "pin")
				{
					assert(pinNode->existTag("direction"));
					if(pinNode->getData("direction") == "input")
					{
						//cout << "addInput" <<endl;
						relativePinIndexMap[pinNode->getName()] = relativePinIndex++;
						assert(pinNode->existTag("rise_capacitance"));
						assert(pinNode->existTag("fall_capacitance"));
						riseCapa.push_back(atof(pinNode->getData("rise_capacitance").c_str()));
						fallCapa.push_back(atof(pinNode->getData("fall_capacitance").c_str()));
					}
					else
					{
						//cout << "addOutput" <<endl;
						pinIndexMap[pinNode->getName()] = pinIndex++;
						//cout << node->getName() << endl;
						//assert(pinNode->existTag("function"));
						if(node->existTag("function"))
							optFunction.push_back(pinNode->getData("function"));
						else
							optFunction.push_back(string());
					}
				}
			}

			//index builder
			cellIndex_[node->getName()] = cellIndex++;
			pinIndex_.push_back(pinIndexMap);
			relativePinIndex_.push_back(relativePinIndexMap);

			//capacitance builder;
			riseCapacitance_.push_back(riseCapa);
			fallCapacitance_.push_back(fallCapa);

			//function builder
			function_.push_back(optFunction);
		}
	}
}

void Library::buildLookUpTable()
{
	//lookUpTable builder
	const vector<LibNode*> childs = root_->getChilds();
	for(vector<LibNode*>::const_iterator n = childs.begin() ; n != childs.end() ; n++)
	{
		LibNode *node = *n;
		if(node->getType() == "cell")
		{
			const vector<LibNode*> childs = node->getChilds();
			
			for(vector<LibNode*>::const_iterator p = childs.begin() ; p != childs.end() ; p++)
			{
				LibNode *pinNode = *p;
				if(pinNode->getType() == "pin" && pinNode->getData("direction") == "output")
				{
					const vector<LibNode*> childs = pinNode->getChilds();
					for(vector<LibNode*>::const_iterator t = childs.begin() ; t != childs.end() ; t++)
					{
						LibNode *timingOrPowerNode = *t;
						if(timingOrPowerNode->getType() == "timing" || timingOrPowerNode->getType() == "internal_power")
						{
							const vector<LibNode*> childs = timingOrPowerNode->getChilds();
							for(vector<LibNode*>::const_iterator l = childs.begin() ; l != childs.end() ; l++)
							{
								LibNode *lutNode = *l;
								if(lutNode->getType() == "cell_fall"       || lutNode->getType() == "cell_rise" || 
								   lutNode->getType() == "fall_transition" || lutNode->getType() == "rise_transition" ||
								   lutNode->getType() == "fall_power"      || lutNode->getType() == "rise_power")
								{
									struct Lut lut;
									lut.templateIndex = templateIndex_[lutNode->getName()];
									lut.node = lutNode;
									const vector<string> &value = lutNode->getDataArray("values");
									stringstream s;
									
									for(unsigned v = 0 ; v < value.size() ; v++)
										s << value[v] << ',';
									char c;
									for(int i = 0 ; i < 7 ; i++)
										for(int j = 0 ; j < 7 ; j++)
											s >> lut.index[i][j] >> c;
									lookUpTables_.push_back(lut);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Library::lutEncode()
{
	int lutId(0);
	
	for(unsigned i = 0 ; i < lookUpTables_.size() ; i++)
	{
		Lut &lut = lookUpTables_[i];
		int cellIndex = cellIndex_[lut.node->getParent()->getParent()->getParent()->getName() ];
		string cellName =  lut.node->getParent()->getParent()->getParent()->getName();
		//cout << "cell type: " << lut.node->getType() <<endl;
		int retType;
		if(lut.node->getType() == "cell_fall")
			retType = 0;
		else if(lut.node->getType() == "cell_rise")
			retType = 1;
		else if(lut.node->getType() == "fall_transition")
			retType = 2;
		else if(lut.node->getType() == "rise_transition")
			retType = 3;
		else if(lut.node->getType() == "fall_power")
			retType = 4;
		else if(lut.node->getType() == "rise_power")
			retType = 5;
		else
			assert(0);
		int pinIndex = pinIndex_[cellIndex][lut.node->getParent()->getParent()->getName()];
		//cout << "pinType: " << lut.node->getParent()->getParent()->getType() << endl;
		int relativePinIndex = relativePinIndex_[cellIndex][lut.node->getParent()->getData("related_pin")];
		//cout << lut.node->getParent()->getData("related_pin")<<endl;
		//cout << endl;
		int signalToLutIndex  = cellIndex*maxFanOutFanIn + relativePinIndex_[cellIndex].size() * pinIndex + relativePinIndex;
		if(lut.node->getParent()->existTag("when"))
		{
			string signal = lut.node->getParent()->getData("when");
			addSig(cellIndex , signal , lutId , signalToLutIndex , retType);
		}
		else
		{
			if(fanInSigToLut_[retType][signalToLutIndex].sigToLut.size() == 0){
				fanInSigToLut_[retType][signalToLutIndex].sigToLut.assign(pow(2 , relativePinIndex_[cellIndex].size()) , lutId);
			}
			else
			{
				for(unsigned i = 0 ; i < fanInSigToLut_[retType][signalToLutIndex].sigToLut.size() ; i++)
					if(fanInSigToLut_[retType][signalToLutIndex].sigToLut[i] == -1)
						fanInSigToLut_[retType][signalToLutIndex].sigToLut[i] = lutId;
			}
		}
		lutId++;
	}
	/*
	for(int i = 0 ; i < fanInSigToLut_.size() ; i++)
		for(int j = 0 ; j < fanInSigToLut_[i].size() ; j++)
			for(int k = 0 ; k < fanInSigToLut_[i][j].sigToLut.size() ; k++)
				if(fanInSigToLut_[i][j].sigToLut[k] == -1)
				{
					cout << "cell index: " << j/maxFanOutFanIn <<" ";
					for(map<string,int>::iterator it = cellIndex_.begin() ; it!=cellIndex_.end() ; it++)
						if(it->second == j/maxFanOutFanIn)
							cout << it->first << endl;
					cout << "cell input amount" << relativePinIndex_[j/maxFanOutFanIn].size() <<endl;
					cout << "relative input id" << j%maxFanOutFanIn <<endl;
					BOOST_FOREACH(int er , fanInSigToLut_[i][j].sigToLut)
						cout << er << " ";
					cout << endl;
					assert(0);
				}
	*/
}

void Library::addSig(int cellIndex , const string &signal , int lutIndex , int signalToLutIndex,int retType)
{
	queue<vector<char> >equalSignals;
	vector<char>buildSignal;
	buildSignal.assign(relativePinIndex_[cellIndex].size() , 'x');
	//cout << "when: " << signal <<endl;
	stringstream ss;
	ss << signal;
	char word[10];
	while(ss >> word)
	{
		if(word[0] == '&')
			continue;

		if(word[0] == '!')
		{
			string pinName(word);
			pinName.erase(pinName.begin());
			int pinIndex = relativePinIndex_[cellIndex][pinName];
			buildSignal[pinIndex] = '0';
		}
		else if(word[0] == '|')
		{
			equalSignals.push(buildSignal);
			//cout <<"convert to signal: " <<endl;
			//for(int i = 0 ; i < buildSignal.size() ; i++)
			//	cout << buildSignal[i];
			//cout <<endl;
			buildSignal.assign(relativePinIndex_[cellIndex].size() , 'x');
		}
		else
		{
			string pinName(word);
			int pinIndex = relativePinIndex_[cellIndex][pinName];
			buildSignal[pinIndex] = '1';
		}
	}
	if(fanInSigToLut_[retType][signalToLutIndex].sigToLut.empty())
		fanInSigToLut_[retType][signalToLutIndex].sigToLut.assign(pow(2 , buildSignal.size()),-1);
	//cout << "size of sigToLut: " << fanInSigToLut_[retType][signalToLutIndex].sigToLut.size() << endl;
	//cout << "size of buildSig: " << buildSignal.size() <<endl;
	assert(fanInSigToLut_[retType][signalToLutIndex].sigToLut.size() == pow(2 , buildSignal.size()));
	equalSignals.push(buildSignal);
	//cout <<"convert to signal: " <<endl;
	//for(int i = 0 ; i < buildSignal.size() ; i++)
	//	cout << buildSignal[i];
	//cout <<endl;
	while(!equalSignals.empty())
	{
		vector<char> sig = equalSignals.front();
		equalSignals.pop();
		bool fillXComplete = true;
		//cout << endl;
		for(unsigned i = 0 ; i < sig.size() ; i++)
		{
			if(sig[i] == 'x')
			{
				fillXComplete = false;
				sig[i] = '0';
				equalSignals.push(sig);
				sig[i] = '1';
				equalSignals.push(sig);
				break;
			}
		}
		if(fillXComplete)
		{
			int encode(0);
			pow(2 , signal.size());
			int num = 1;
			//cout << "find complete: " ;
			for(unsigned i = 0 ; i < sig.size() ; i++)
			{
				//cout << sig[i]; 
				if(sig[i] == '1')
					encode+=num;
				num*=2;
			}
			//cout << endl;
			//cout << "fill: " << encode <<endl;
			fanInSigToLut_[retType][signalToLutIndex].sigToLut[encode] = lutIndex;
		}
	}
	//cout << "size :" << fanInSigToLut_[retType][signalToLutIndex].sigToLut.size() <<endl;
	//cout <<endl;
}

double Library::get(LookUpTableInfo &info)
{
	int fanInSignToLutIndex = info.cellIndex * maxFanOutFanIn + relativePinIndex_[info.cellIndex].size() * info.pinIndex + info.relativePinIndex;
	FanInSigToLut &sigToLut = fanInSigToLut_[info.returnType + info.toggleRise][fanInSignToLutIndex];
	int lutIndex = (sigToLut.sigToLut.size() == 1)?sigToLut.sigToLut[0] :sigToLut.sigToLut[info.faninSignal];
	Lut &lut = lookUpTables_[lutIndex];
	int x1 , y1;
	vector<double> &index1Time = template_[lut.templateIndex];
	vector<double> &index2Capa = template_[lut.templateIndex+1];
	//cout << "template number: " << lut.templateIndex <<endl;
	// index1 matching y
	for(y1 = 5 ; index1Time[y1] > info.inputTransition && y1 != 0; y1-=1);
	
	// index2 matching x
	for(x1 = 5 ; index2Capa[x1] > info.capacitance && x1 != 0; x1-=1);

	int x2 = x1+1;
	int y2 = y1+1;
	//cout << "table: " << lut.index[0][0] << endl;
	//cout << "index: " << x1 << " "<< y1 <<endl;
	//for(int i = 0 ; i < 7 ; i++)
	//	cout << index1Time[i]<< " ";
	//cout <<endl;
	double pLeftUp = lut.index[y1][x1];
	double pRightUp = lut.index[y1][x2];
	double pRightDown = lut.index[y2][x2];
	double pLeftDown = lut.index[y2][x1];
	
	//cout << pLeftUp << " " << pRightUp << " " << pRightDown << " " << pLeftDown <<endl;

	double persentageX = (info.capacitance - index2Capa[x1])/(index2Capa[x2] - index2Capa[x1]);
	double persentageY = (info.inputTransition - index1Time[y1])/(index1Time[y2] - index1Time[y1]);
	
	//cout << "inputTransition: " << info.inputTransition << endl;
	//cout << "tableTransition: " << index1Time[y1] << endl;
	//cout << persentageX << " " << persentageY <<endl;

	double middle1 = (pRightUp - pLeftUp)*persentageX + pLeftUp;
	double middle2 = (pRightDown - pLeftDown)*persentageX + pLeftDown;
	return 			 (middle2 - middle1)*persentageY + middle1;
}


double Library::get(LookUpTableInfoString &info)
{
	LookUpTableInfo info2;
	info2.returnType = info.returnType;
	info2.cellIndex = getCellIndex(info.cellType);
	info2.pinIndex = getFanOutIndex(info2.cellIndex , info.pinName);
	info2.relativePinIndex = getFanInIndex(info2.cellIndex , info.relativePinName);
	info2.faninSignal = 0;
	//cout << "cellIndex" << info2.cellIndex <<endl;
	//cout << "pinIndex " << info2.pinIndex <<endl;
	for(unsigned i = 0 ; i <info.fanInSignal.size() ; i++)
	{
		int id = getFanInIndex(info2.cellIndex , info.fanInSignal[i]);
		info2.faninSignal+=pow(2 , id);
	}
	info2.toggleRise = info.toggleRise;
	info2.capacitance = info.capacitance;
	info2.inputTransition = info.inputTransition;
	return get(info2);
}


int Library::getCellIndex(const std::string &cellType)
{
	std::map<std::string , int>::iterator finder = cellIndex_.find(cellType);
	if(finder == cellIndex_.end())
		return -1;
	return finder->second;;
}


int Library::getFanOutIndex(int cellTypeIndex , const std::string &pinName)
{
	std::map<std::string , int>::iterator finder = pinIndex_[cellTypeIndex].find(pinName);
	if(finder == pinIndex_[cellTypeIndex].end())
		return -1;
	//cout << "find!!!!!!"<<finder->second<<endl;
	return finder->second;
}


int Library::getFanInIndex(int cellTypeIndex , const std::string &relativePinName)
{
	std::map<std::string , int>::iterator finder = relativePinIndex_[cellTypeIndex].find(relativePinName);
	if(finder ==relativePinIndex_[cellTypeIndex].end())
		return -1;
	return relativePinIndex_[cellTypeIndex][relativePinName];
}

double Library::getCapacitance(LookUpTableInfo &info)
{
	assert(info.cellIndex < (int)riseCapacitance_.size());
	assert(info.relativePinIndex < (int)riseCapacitance_[info.cellIndex].size());
	if(info.toggleRise)
		return riseCapacitance_[info.cellIndex][info.relativePinIndex];
	else
		return fallCapacitance_[info.cellIndex][info.relativePinIndex];
}

bool Library::isFanOut(int cellIndex , const std::string &pinName)
{
	std::map<std::string , int>::iterator finder = pinIndex_[cellIndex].find(pinName);
	return finder != pinIndex_[cellIndex].end();
}


bool Library::isFanIn(int cellIndex , const std::string &pinName)
{
	std::map<std::string , int>::iterator finder = relativePinIndex_[cellIndex].find(pinName);
	return finder != relativePinIndex_[cellIndex].end();
}

int Library::getFanOutSize(int cellIndex)
{
	return pinIndex_[cellIndex].size();
}
int Library::getFanInSize(int cellIndex)
{
	return relativePinIndex_[cellIndex].size();
}
