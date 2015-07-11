#ifndef __IDEA_REATPG_H__
#define __IDEA_REATPG_H__

#include <string>
#include <set>
#include <fstream>

class PatternSet;
class TetraMax;

class ReAtpg
{
public:
	ReAtpg();
	~ReAtpg();
	bool setRisk(int riskPatID);

	void cleanWorkSpace(const std::string &workSpace);
	bool writeSaveRiskWsl(const std::string &wglFile , const std::string &reGeneWorkSpace);
	std::string getSafeWgl() { return safeWglAddress; };
	std::string getRiskWgl() { return riskWglAddress; };
	// return the file address of newPattern.pat and newPattern.wgl
	// usage: string address = reGene();
	//        string patAddress = address + ".pat";
	//        string wglAddress = address + ".wgl";
	std::string faultSim1Stage(const std::string &reGeneWorkSpace , TetraMax *tmax);
	std::string faultSim2Stage(const std::string &reGeneWorkSpace , TetraMax *tmax);
	std::string regene(const std::string &reGeneWorkSpace , TetraMax *tmax , const std::string &undetect);

private:

	std::set<int> riskPatIDs_;

	std::string safeWglAddress;
	std::string riskWglAddress;
	std::string undetectFlt;
	std::string writePat(std::ofstream &pats , std::ifstream &all);
};

#endif
