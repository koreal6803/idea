
// **************************************************************************
// File       [ scan_chain_info ]
// Author     [ koreal ]
// Synopsis   [ ]
// Date       [ 2014/11/06 created ]
// **************************************************************************
#include <string>
#include <vector>

class ScanChainInfo
{
public:
	std::string clk;
	std::string enable;
	std::vector<std::string> scan_in_pins;
	std::vector<std::string> scan_out_pins;
	std::vector<std::string> scan_ff;//type of scan flip flop
	std::string scan_ff_si;
	std::vector<std::string> scan_ff_so;
};
