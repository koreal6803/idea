#ifndef __IR_CORE_POWER_GRID_NODE__
#define __IR_CORE_POWER_GRID_NODE__

#include <vector>
#include <string>
#include "stdlib.h"
#include <math.h>
namespace pgNs
{

class Node
{
	friend class PowerGrid;
public:

	enum Type {POWER , GROUND , UN_SET};
	Node				()			{pad_ = false; type_ = UN_SET; voltage_ = -1; current_ = 0;}
	std::string getName	()const		{return name_;}
	Type getType		()const		{return type_;}
	bool isPad			()const		{return pad_;}
	double getVoltage	()const		{return voltage_;}
	double getCurrent	()const		{return current_;}
	void setVoltage		(double v)	{voltage_ = v;}
	void setCurrent		(double i)	{current_ = fabs(i);}
	double x;
	double y;
private:
	std::string name_;
	
	//UN_SET usually won't happen after the power grid is build
	//POWER means the node is belong to power supply network
	//GROUND means the node is belong to ground network
	
	Type type_;
	
	//pad == true meas this node's voltage is always be VDD(1.1) or GND(0)
	bool pad_;

	double voltage_;
	double current_;

};
};

#endif
