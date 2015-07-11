#include "power_grid_solver.h"
#include "power_grid.h"
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
using namespace std;
using namespace matrixNs;
using namespace pgNs;
PowerGridSolver::PowerGridSolver(PowerGrid *pg , bool onlyR, double stepSize)
{
	powerGrid = pg;
    timeStep = stepSize;
	onlyR_ = onlyR;
	int voltageSourceSize(0);
	int inductanceSize(0);
	int capacitanceSize(0);
	const vector<Component> &components = pg->getComponents();
	for(unsigned i = 0 ; i < components.size() ; i++)
		switch(components[i].getType())
		{
			case Component::VS:
				voltageSourceSize++;
				break;
			case Component::IND:
				inductanceSize++;
				break;
			case Component::CAP:
				capacitanceSize++;
				break;
			default:
				break;
		}
	/*
	cout << "size" <<endl;
	cout << voltageSourceSize << endl;
	cout << inductanceSize <<endl;
	cout << capacitanceSize <<endl;
	cout << "matrixSize " << matrixSize <<endl;
	cout << "nodeSize   " << pg->getNodes().size()<<endl;
	getchar();
	*/


	//initial matrix
	//
	//if components are voltage source or inductance, they will need a new variable(current).
	//and the matrix size will increase
	int matrixSize = pg->getNodes().size() + voltageSourceSize + inductanceSize;
	matrixG = new Matrix(matrixSize);
	matrixC = new Matrix(matrixSize);
	voltage = new Vector(matrixSize);
	//link row to the component
	//link the voltage source and inductance to their current col and row
	linkRow();

	//set initial voltage from power grid to vector "voltage"
	setInitialVoltage();
	
	//build matrix C and G
	buildMatrixCG();
	
	//build matrix LHS
	matrixLHS = new Matrix(*matrixG);
	*matrixC /= stepSize;
	*matrixLHS += *matrixC;
//    matrixLHS->print();
//    getchar();

}
PowerGridSolver::~PowerGridSolver()
{
	delete matrixG;
	delete matrixLHS;
	delete matrixC;
	delete voltage;
    //delete prevVol;
    //delete prevPrevVol;
}
void PowerGridSolver::setTimeStep(double t)
{
    *matrixLHS -= *matrixC;
    *matrixC/=(1/timeStep);
    
    timeStep = t;
    
    *matrixC/=timeStep;
    *matrixLHS += *matrixC;
}

void PowerGridSolver::linkRow()
{
	vector<Node> const &pgNodes = powerGrid->getNodes();
	
	int row = pgNodes.size();
	const vector<Component> &components = powerGrid->getComponents();

	//if components are voltage source or inductance, they will need a new variable.
	//and the matrix size will increase
	for(unsigned i = 0 ; i < components.size() ; ++i){
		if(components[i].getType() == Component::VS)
			comIndexToRow[i] = row++;
		else if(components[i].getType() == Component::IND)
			comIndexToRow[i] = row++;
	}
}

void PowerGridSolver::setInitialVoltage()
{
	const vector<Component> &components = powerGrid->getComponents();
	//voltage initialize
	const vector<Node> nodes = powerGrid->getNodes();
	//set node voltage

	for(unsigned i = 0 ; i < components.size() ; ++i)
	{
		const Component &com = components[i];
		switch(com.getType())
		{
			case Component::VS:
				voltage->set(comIndexToRow[i] , powerGrid->getSupplyVoltage());
				break;
			default:
				break;
		}
	}
}

void PowerGridSolver::buildMatrixCG()
{
	const vector<Component> &components = powerGrid->getComponents();
	Matrix &mg = *matrixG;
	Matrix &mc = *matrixC;
	//build matrix G and C
	for(unsigned c = 0 ; c < components.size(); c++)
	{
		const Component &com = components[c];
		switch(com.getType())
		{
			case Component::RES:
			{
				mg[com.getVPos()][com.getVPos()] += 1/com.getValue();
				
				assert(com.getVNeg() != -1);
				
				mg[com.getVNeg()][com.getVNeg()] += 1/com.getValue();

				mg[com.getVPos()][com.getVNeg()] +=-1/com.getValue();
				mg[com.getVNeg()][com.getVPos()] +=-1/com.getValue();
				break;
			}
			case Component::VS:
			{
				assert(comIndexToRow.find(c) != comIndexToRow.end());
				int currentRow = comIndexToRow[c];
				
				mg[currentRow][com.getVPos()] = 1;
				mg[com.getVPos()][currentRow] = 1;
				//mg[com.getVPos()][com.getVPos()] = 0.000001;
				//mg[currentRow][currentRow] = 0.000001;

				if(com.getVNeg() == -1)
					break;
				//mg[com.getVNeg()][com.getVNeg()] = 0.000001;
				
				mg[currentRow][com.getVNeg()] = -1;
				mg[com.getVNeg()][currentRow] = -1;
				break;
			}
			case Component::CAP:
			{
				if(onlyR_)
					break;
				mc[com.getVPos()][com.getVPos()] += com.getValue();
				if(com.getVNeg() == -1)
					break;

				mc[com.getVNeg()][com.getVNeg()] += com.getValue();
				mc[com.getVNeg()][com.getVPos()] += -com.getValue();
				mc[com.getVPos()][com.getVNeg()] += -com.getValue();
				break;
			}
			case Component::IND:
			{
				if(onlyR_)
					break;
				assert(comIndexToRow.find(c) != comIndexToRow.end());
				int currentRow = comIndexToRow[c];
				mg[currentRow][com.getVPos()] += 1;
				mg[com.getVPos()][currentRow] += 1;
				mc[currentRow][currentRow] += -com.getValue();
				if(com.getVNeg() == -1)
					break;
				
				mg[currentRow][com.getVNeg()] += -1;
				mg[com.getVNeg()][currentRow] += -1;
			}
			default:
				break;
		}
	}


			
	//matrixG->print();

}


bool PowerGridSolver::solve(Vector *current)
{

	// the current source of power node should be nagtive
	// the current source of ground node should be positive 
	// the current vector size should match the matrix dimision
	//set voltage source

	const vector<Component> &components = powerGrid->getComponents();
	for(unsigned i = 0 ; i < components.size() ; ++i)
	{
		const Component &com = components[i];
		switch(com.getType())
		{
			case Component::VS:
				current->set(comIndexToRow[i] , com.getValue());
				break;
			default:
				break;
		}
	}
	
    //build rhs
	if(onlyR_)
	{
		bool success = matrixNs::solve(matrixG , current);
		
		if(!success){
			cout << "**WARRN KLU: solve fail!" << endl;
			return false;
		}
	}
	else
	{
		*voltage = *matrixC * *voltage;
		*voltage += *current;
		bool success = matrixNs::solve(matrixLHS , voltage);

		if(!success){
			cout << "**WARRN KLU: solve fail!" << endl;
			return false;
		}
		//solve success
		for(unsigned i = 0 ; i < powerGrid->getNodes().size() ; i++){
			powerGrid->getNodes()[i].setVoltage(voltage->get(i));
		}
	}

	return true;
}

bool PowerGridSolver::solve()
{
	//set current of Nodes
	Vector newVec(int(voltage->size()));
	vector<Node> const &pgNodes = powerGrid->getNodes();
	for(unsigned i = 0 ; i < pgNodes.size() ; i++)
		if(pgNodes[i].getType() == Node::POWER)
			newVec.set(i , - pgNodes[i].getCurrent());
		else if(pgNodes[i].getType() == Node::GROUND)
			newVec.set(i , pgNodes[i].getCurrent());
	
	//set voltage source
	const vector<Component> &components = powerGrid->getComponents();
	for(unsigned i = 0 ; i < components.size() ; ++i)
	{
		const Component &com = components[i];
		switch(com.getType())
		{
			case Component::VS:
				newVec.set(comIndexToRow[i] , com.getValue());
				break;
			default:
				break;
		}
	}
	
    //build rhs
	if(onlyR_)
	{
		bool success = matrixNs::solve(matrixG , &newVec);
		
		if(!success){
			cout << "**WARRN KLU: solve fail!" << endl;
			return false;
		}

		//solve success
		for(unsigned i = 0 ; i < powerGrid->getNodes().size() ; i++){
			powerGrid->getNodes()[i].setVoltage(newVec.get(i));
		}
	}
	else
	{
		*voltage = *matrixC * *voltage;
		*voltage += newVec;
		bool success = matrixNs::solve(matrixLHS , voltage);

		if(!success){
			cout << "**WARRN KLU: solve fail!" << endl;
			return false;
		}
		//solve success
		for(unsigned i = 0 ; i < powerGrid->getNodes().size() ; i++){
			powerGrid->getNodes()[i].setVoltage(voltage->get(i));
		}
	}

	return true;
}

