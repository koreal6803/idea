#include "delay_info_builder.h"
#include <cstdio>
#include <cassert>
#include <algorithm>
#include "delay_info.h"
#include "circuit.h"
#include "library.h"
using namespace std;


DelayInfoBuilder::DelayInfoBuilder()
{
	delay = new DelayInfo;
	cir = NULL;
	lib = NULL;
}

DelayInfoBuilder::~DelayInfoBuilder()
{
	delete delay;
}

void DelayInfoBuilder::set(Circuit *cir)
{
	this->cir = cir;
	
	const vector<Cell> &cells = cir->getCells();

	//delay initiate
	delay->riseGateDelay.assign(cells.size(), vector<double>());
	delay->fallGateDelay.assign(cells.size(), vector<double>());
	delay->iptSize.assign(cells.size(), 0);
	delay->riseNetDelay.assign(cells.size(), vector<double>());
	delay->fallNetDelay.assign(cells.size(), vector<double>());
	for(unsigned cellID = 0 ; cellID < cells.size() ; ++cellID)
	{
		int optMaxID = cells[cellID].opt_net_id.size();
		int iptMaxID = cells[cellID].ipt_net_id.size();

		// ipt size
		delay->iptSize.push_back(iptMaxID);

		// gate delay
		delay->riseGateDelay[cellID].assign(optMaxID*iptMaxID,0);
		delay->fallGateDelay[cellID].assign(optMaxID*iptMaxID,0);

		// net delay
		delay->riseNetDelay[cellID].assign(iptMaxID,0);
		delay->fallNetDelay[cellID].assign(iptMaxID,0);

	}
}

void DelayInfoBuilder::set(Library *lib)
{
	this->lib = lib;
}

bool DelayInfoBuilder::read(const char * const fname) {
	
	if(lib == NULL)
	{
		cout << "DelayInfoBuilder: please set the library before read the sdf file" <<endl;
		return false;
	}
	if(cir == NULL)
	{
		cout << "DelayInfoBuilder: please set the circuit before read the sdf file" <<endl;
		return false;
	}

    extern int sdf_fileparse(void *);
    extern FILE *sdf_filein;

    verbose_ = false;
    sdf_filein = fopen(fname, "r");
    if (!sdf_filein) {
        fprintf(stderr, "**ERROR SdfFile::read(): cannot open netlist file");
        fprintf(stderr, "`%s'\n", fname);
        return false;
    }

    success_ = true;
    int res = sdf_fileparse(this);
    if (res != 0) {
        fprintf(stderr, "**ERROR SdfFile::read(): wrong input format\n");
        fclose(sdf_filein);
        return false;
    }
    fclose(sdf_filein);

    return success_;
}

bool DelayInfoBuilder::addInterconnectDelay(const SdfDelayType &type
	, const char * const from
	, const char * const to
	, const SdfDelayValueList &v)
{
	string info(to);
	std::size_t found = info.find('/');
	if(found == string::npos)
		return true;
	string cellName = info.substr(0,found);
	string pinName = info.substr(found+1);
	

	int cellID = cir->getCellID(cellName);
	int ty = cir->getCells()[cellID].type;
	int pinID = -1;
	assert(lib->isFanIn(ty , pinName));
	pinID = lib->getFanInIndex(ty , pinName);
	/*	
	cout << "cell name " << cellName <<endl;
	cout << "pin  name " << pinName <<endl;
	cout << "cell ID " << cellID <<endl;
	cout << "pin  ID " << pinID <<endl;
	cout << "rise delay: " <<  v.v[0].v[0].v[1]<<endl;
	cout << "fall delay: " <<  v.v[1].v[0].v[1]<<endl;
	cout << "rise net delay size " << delay->riseNetDelay[cellID].size() <<endl;
	cout << "fall net delay size " << delay->fallNetDelay[cellID].size() <<endl;
	cout << "-------------------------------------------" <<endl;
	*/
	delay->riseNetDelay[cellID][pinID] = v.v[0].v[0].v[1];
	delay->fallNetDelay[cellID][pinID] = v.v[1].v[0].v[1];
	return true;
}
bool DelayInfoBuilder::addCell(const char * const type, const char * const name)
{
	if(string(name) == "")
		return true;
	presentCellID = cir->getCellID(string(name));
	return true;
}
bool DelayInfoBuilder::addIoDelay(const SdfDelayType &type
	, const SdfPortSpec &spec
	, const char * const port
	, const SdfDelayValueList &v)
{
	string iptPort = spec.port;
	string optPort = port;
	
	double riseDelay = v.v[0].v[0].v[1];
	double fallDelay = v.v[1].v[0].v[1];
	int cellType = cir->getCells()[presentCellID].type;
	int iptSize = cir->getCells()[presentCellID].ipt_net_id.size();
	int iptPortID = lib->getFanInIndex(cellType , iptPort);
	int optPortID = lib->getFanOutIndex(cellType , optPort);
	int encode = optPortID * iptSize + iptPortID;
	delay->riseGateDelay[presentCellID][encode] = riseDelay;
	delay->fallGateDelay[presentCellID][encode] = fallDelay;
	
	return true;
}

DelayInfo DelayInfoBuilder::getDelayInfo()
{
	return *delay;
}
