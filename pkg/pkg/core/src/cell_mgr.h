// **************************************************************************
// File       [ cell_mgr.h ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************

#ifndef __INTF_CELL_MGR_H__
#define __INTF_CELL_MGR_H__

#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>
		using namespace std;
class CellMgr
{
    std::map<std::string,int> cellID;
    std::vector<std::map<std::string,int> > pinID;
    std::vector<std::map<std::string,int> > isOutput;

	std::vector<std::string> typeID;
	std::vector<std::vector<std::string> > pinName;

public:
	int getTypeID(std::string &type)
	{
		assert(cellID.find(type)!= cellID.end());
		return cellID[type];
	}

	std::string getTypeName(int typeID)
	{
		return this->typeID[typeID];
	}

	int getPinID(int type , std::string pin)
	{
		assert(type >= 0);
		assert((unsigned)type < pinID.size());
		assert(pinID[type].find(pin) != pinID[type].end());
		return pinID[type][pin];
	}
	
	bool pinIsOutput(int type , std::string pin)
	{
		assert(type >= 0);
		assert((unsigned)type < isOutput.size());
		assert(isOutput[type].find(pin) != isOutput[type].end());
		return (isOutput[type][pin] == 1);
	}
	
	const std::string &getPinName(int cellID , int PinID)
	{
		assert((int)pinName.size() > cellID);
		assert((int)pinName[cellID].size() > PinID);
		return pinName[cellID][PinID];
	}

	
    CellMgr(){
        cellID["AND2_X1"] = 0;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;
        
		cellID["AND2_X2"] = 1;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["AND2_X4"] = 2;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["AND3_X1"] = 3;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AND3_X2"] = 4;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AND3_X4"] = 5;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AND4_X1"] = 6;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AND4_X2"] = 7;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AND4_X4"] = 8;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["ANTENNA_X1"] = 9;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;

        cellID["AOI211_X1"] = 10;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AOI211_X2"] = 11;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AOI211_X4"] = 12;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AOI21_X1"] = 13;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AOI21_X2"] = 14;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AOI21_X4"] = 15;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["AOI221_X1"] = 16;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["AOI221_X2"] = 17;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["AOI221_X4"] = 18;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["AOI222_X1"] = 19;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["AOI222_X2"] = 20;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["AOI222_X4"] = 21;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["AOI22_X1"] = 22;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AOI22_X2"] = 23;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["AOI22_X4"] = 24;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["BUF_X1"] = 25;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["BUF_X16"] = 26;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["BUF_X2"] = 27;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["BUF_X32"] = 28;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["BUF_X4"] = 29;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["BUF_X8"] = 30;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["CLKBUF_X1"] = 31;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["CLKBUF_X2"] = 32;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["CLKBUF_X3"] = 33;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["Z"] = 1;
          isOutput.back()["Z"] = 1;

        cellID["CLKGATETST_X1"] = 34;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["SE"] = 3;
          isOutput.back()["SE"] = 0;
          pinID.back()["GCK"] = 4;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATETST_X2"] = 35;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["SE"] = 3;
          isOutput.back()["SE"] = 0;
          pinID.back()["GCK"] = 4;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATETST_X4"] = 36;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["SE"] = 3;
          isOutput.back()["SE"] = 0;
          pinID.back()["GCK"] = 4;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATETST_X8"] = 37;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["SE"] = 3;
          isOutput.back()["SE"] = 0;
          pinID.back()["GCK"] = 4;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATE_X1"] = 38;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["GCK"] = 3;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATE_X2"] = 39;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["GCK"] = 3;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATE_X4"] = 40;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["GCK"] = 3;
          isOutput.back()["GCK"] = 1;

        cellID["CLKGATE_X8"] = 41;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["IQ"] = 0;
          isOutput.back()["IQ"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["E"] = 2;
          isOutput.back()["E"] = 0;
          pinID.back()["GCK"] = 3;
          isOutput.back()["GCK"] = 1;

        cellID["DFFRS_X1"] = 42;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SN"] = 2;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 3;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 4;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 5;
          isOutput.back()["QN"] = 1;

        cellID["DFFRS_X2"] = 43;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SN"] = 2;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 3;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 4;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 5;
          isOutput.back()["QN"] = 1;

        cellID["DFFR_X1"] = 44;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["CK"] = 2;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 3;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 4;
          isOutput.back()["QN"] = 1;

        cellID["DFFR_X2"] = 45;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["CK"] = 2;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 3;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 4;
          isOutput.back()["QN"] = 1;

        cellID["DFFS_X1"] = 46;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SN"] = 1;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 2;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 3;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 4;
          isOutput.back()["QN"] = 1;

        cellID["DFFS_X2"] = 47;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SN"] = 1;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 2;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 3;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 4;
          isOutput.back()["QN"] = 1;

        cellID["DFF_X1"] = 48;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 3;
          isOutput.back()["QN"] = 1;

        cellID["DFF_X2"] = 49;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["CK"] = 1;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 3;
          isOutput.back()["QN"] = 1;

        cellID["DLH_X1"] = 50;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["G"] = 1;
          isOutput.back()["G"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;

        cellID["DLH_X2"] = 51;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["G"] = 1;
          isOutput.back()["G"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;

        cellID["DLL_X1"] = 52;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["GN"] = 1;
          isOutput.back()["GN"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;

        cellID["DLL_X2"] = 53;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["GN"] = 1;
          isOutput.back()["GN"] = 0;
          pinID.back()["Q"] = 2;
          isOutput.back()["Q"] = 1;

        cellID["FA_X1"] = 54;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["CI"] = 2;
          isOutput.back()["CI"] = 0;
          pinID.back()["CO"] = 3;
          isOutput.back()["CO"] = 1;
          pinID.back()["S"] = 4;
          isOutput.back()["S"] = 1;

        cellID["FILLCELL_X1"] = 55;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["FILLCELL_X16"] = 56;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["FILLCELL_X2"] = 57;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["FILLCELL_X32"] = 58;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["FILLCELL_X4"] = 59;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["FILLCELL_X8"] = 60;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());

        cellID["HA_X1"] = 61;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["CO"] = 2;
          isOutput.back()["CO"] = 1;
          pinID.back()["S"] = 3;
          isOutput.back()["S"] = 1;

        cellID["INV_X1"] = 62;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["INV_X16"] = 63;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["INV_X2"] = 64;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["INV_X32"] = 65;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["INV_X4"] = 66;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["INV_X8"] = 67;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["ZN"] = 1;
          isOutput.back()["ZN"] = 1;

        cellID["LOGIC0_X1"] = 68;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["Z"] = 0;
          isOutput.back()["Z"] = 1;

        cellID["LOGIC1_X1"] = 69;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["Z"] = 0;
          isOutput.back()["Z"] = 1;

        cellID["MUX2_X1"] = 70;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["S"] = 2;
          isOutput.back()["S"] = 0;
          pinID.back()["Z"] = 3;
          isOutput.back()["Z"] = 1;

        cellID["MUX2_X2"] = 71;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["S"] = 2;
          isOutput.back()["S"] = 0;
          pinID.back()["Z"] = 3;
          isOutput.back()["Z"] = 1;

        cellID["NAND2_X1"] = 72;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NAND2_X2"] = 73;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NAND2_X4"] = 74;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NAND3_X1"] = 75;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NAND3_X2"] = 76;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NAND3_X4"] = 77;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NAND4_X1"] = 78;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["NAND4_X2"] = 79;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["NAND4_X4"] = 80;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["NOR2_X1"] = 81;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NOR2_X2"] = 82;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NOR2_X4"] = 83;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["NOR3_X1"] = 84;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NOR3_X2"] = 85;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NOR3_X4"] = 86;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["NOR4_X1"] = 87;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["NOR4_X2"] = 88;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["NOR4_X4"] = 89;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI211_X1"] = 90;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI211_X2"] = 91;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI211_X4"] = 92;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["C1"] = 2;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 3;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI21_X1"] = 93;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OAI21_X2"] = 94;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OAI21_X4"] = 95;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OAI221_X1"] = 96;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["OAI221_X2"] = 97;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["OAI221_X4"] = 98;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B1"] = 1;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 2;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 3;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 4;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 5;
          isOutput.back()["ZN"] = 1;

        cellID["OAI222_X1"] = 99;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["OAI222_X2"] = 100;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["OAI222_X4"] = 101;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["C1"] = 4;
          isOutput.back()["C1"] = 0;
          pinID.back()["C2"] = 5;
          isOutput.back()["C2"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["OAI22_X1"] = 102;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI22_X2"] = 103;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI22_X4"] = 104;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["B1"] = 2;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 3;
          isOutput.back()["B2"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OAI33_X1"] = 105;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["B1"] = 3;
          isOutput.back()["B1"] = 0;
          pinID.back()["B2"] = 4;
          isOutput.back()["B2"] = 0;
          pinID.back()["B3"] = 5;
          isOutput.back()["B3"] = 0;
          pinID.back()["ZN"] = 6;
          isOutput.back()["ZN"] = 1;

        cellID["OR2_X1"] = 106;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["OR2_X2"] = 107;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["OR2_X4"] = 108;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["OR3_X1"] = 109;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OR3_X2"] = 110;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OR3_X4"] = 111;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["ZN"] = 3;
          isOutput.back()["ZN"] = 1;

        cellID["OR4_X1"] = 112;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OR4_X2"] = 113;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["OR4_X4"] = 114;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A1"] = 0;
          isOutput.back()["A1"] = 0;
          pinID.back()["A2"] = 1;
          isOutput.back()["A2"] = 0;
          pinID.back()["A3"] = 2;
          isOutput.back()["A3"] = 0;
          pinID.back()["A4"] = 3;
          isOutput.back()["A4"] = 0;
          pinID.back()["ZN"] = 4;
          isOutput.back()["ZN"] = 1;

        cellID["SDFFRS_X1"] = 115;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SE"] = 2;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 3;
          isOutput.back()["SI"] = 0;
          pinID.back()["SN"] = 4;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 5;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 6;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 7;
          isOutput.back()["QN"] = 1;

        cellID["SDFFRS_X2"] = 116;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SE"] = 2;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 3;
          isOutput.back()["SI"] = 0;
          pinID.back()["SN"] = 4;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 5;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 6;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 7;
          isOutput.back()["QN"] = 1;

        cellID["SDFFR_X1"] = 117;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SE"] = 2;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 3;
          isOutput.back()["SI"] = 0;
          pinID.back()["CK"] = 4;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 5;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 6;
          isOutput.back()["QN"] = 1;

        cellID["SDFFR_X2"] = 118;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["RN"] = 1;
          isOutput.back()["RN"] = 0;
          pinID.back()["SE"] = 2;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 3;
          isOutput.back()["SI"] = 0;
          pinID.back()["CK"] = 4;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 5;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 6;
          isOutput.back()["QN"] = 1;

        cellID["SDFFS_X1"] = 119;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SE"] = 1;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 2;
          isOutput.back()["SI"] = 0;
          pinID.back()["SN"] = 3;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 4;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 5;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 6;
          isOutput.back()["QN"] = 1;

        cellID["SDFFS_X2"] = 120;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SE"] = 1;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 2;
          isOutput.back()["SI"] = 0;
          pinID.back()["SN"] = 3;
          isOutput.back()["SN"] = 0;
          pinID.back()["CK"] = 4;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 5;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 6;
          isOutput.back()["QN"] = 1;

        cellID["SDFF_X1"] = 121;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SE"] = 1;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 2;
          isOutput.back()["SI"] = 0;
          pinID.back()["CK"] = 3;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 4;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 5;
          isOutput.back()["QN"] = 1;

        cellID["SDFF_X2"] = 122;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["SE"] = 1;
          isOutput.back()["SE"] = 0;
          pinID.back()["SI"] = 2;
          isOutput.back()["SI"] = 0;
          pinID.back()["CK"] = 3;
          isOutput.back()["CK"] = 0;
          pinID.back()["Q"] = 4;
          isOutput.back()["Q"] = 1;
          pinID.back()["QN"] = 5;
          isOutput.back()["QN"] = 1;

        cellID["TBUF_X1"] = 123;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["EN"] = 1;
          isOutput.back()["EN"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["TBUF_X16"] = 124;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["EN"] = 1;
          isOutput.back()["EN"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["TBUF_X2"] = 125;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["EN"] = 1;
          isOutput.back()["EN"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["TBUF_X4"] = 126;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["EN"] = 1;
          isOutput.back()["EN"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["TBUF_X8"] = 127;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["EN"] = 1;
          isOutput.back()["EN"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["TINV_X1"] = 128;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["EN"] = 0;
          isOutput.back()["EN"] = 0;
          pinID.back()["I"] = 1;
          isOutput.back()["I"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["TLAT_X1"] = 129;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["D"] = 0;
          isOutput.back()["D"] = 0;
          pinID.back()["G"] = 1;
          isOutput.back()["G"] = 0;
          pinID.back()["OE"] = 2;
          isOutput.back()["OE"] = 0;
          pinID.back()["Q"] = 3;
          isOutput.back()["Q"] = 1;

        cellID["XNOR2_X1"] = 130;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["XNOR2_X2"] = 131;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["ZN"] = 2;
          isOutput.back()["ZN"] = 1;

        cellID["XOR2_X1"] = 132;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;

        cellID["XOR2_X2"] = 133;
          pinID.push_back(std::map<std::string,int>());
          isOutput.push_back(std::map<std::string,int>());
          pinID.back()["A"] = 0;
          isOutput.back()["A"] = 0;
          pinID.back()["B"] = 1;
          isOutput.back()["B"] = 0;
          pinID.back()["Z"] = 2;
          isOutput.back()["Z"] = 1;
		
		typeID.assign(cellID.size() , std::string());
		for(std::map<std::string,int>::iterator it = cellID.begin() ; it != cellID.end() ; it++)
			typeID[it->second] = it->first;
		for(unsigned i = 0 ; i < pinID.size() ; i++)
		{
			pinName.push_back(std::vector<std::string>());
			pinName.back().assign(pinID[i].size(),std::string());
			for(std::map<std::string,int>::iterator it = pinID[i].begin() ; it != pinID[i].end() ; it++)
			{
				int id = it->second;
				std::string name = it->first;
				assert((int)pinName[i].size() > id);
				pinName.back()[id] = name;
			}
		}
	}
};
#endif

