#ifndef __IR_CORE_COMPONENT__
#define __IR_CORE_COMPONENT__


namespace pgNs
{

class Component
{
	friend class PowerGrid;
public:
	
	//RES = resistor
	//CAP = capacitance
	//IND = inductance
	//VS =  voltage source
	enum Type {RES , CAP , IND , VS};
	
	Type 	getType()	const{return type_;}
	double 	getValue()	const{return value_;}
	int 	getVPos()	const{return vPos_;}
	int 	getVNeg()	const{return vNeg_;}

private:

	Type 	type_;

	//value means R = 0.1 (omn)
	double 	value_;

	//connected node id
	int 	vPos_;
	int 	vNeg_;

};


};
#endif
