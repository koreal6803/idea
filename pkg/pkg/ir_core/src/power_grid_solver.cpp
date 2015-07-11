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
	for(int i = 0 ; i < components.size() ; i++)
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
    prevVol = new Vector(matrixSize);
    prevPrevVol = new Vector(matrixSize);
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
	for(int i = 0 ; i < components.size() ; ++i){
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
	bool initialize = true;
	for(int i = 0 ; i < nodes.size() ; i++)
	{
		double voltage = nodes[i].getVoltage();
		if(voltage == -1) 
		{
			initialize = false;
			//cout << "PowerGridSolver::Set power  network to VDD" << endl;
			//cout << "PowerGridSolver::Set ground network to GND" <<endl;
			break;
		}
	}
	if(initialize)
	{
		for(int i = 0 ; i < nodes.size() ; i++)
			voltage->set(i , nodes[i].getVoltage());
	}
	else
	{
		for(int i = 0 ; i < nodes.size() ; i++)
		{
			if(nodes[i].getType() == Node::POWER)
			{
				voltage->set(i , powerGrid->getSupplyVoltage());
			}
			else if(nodes[i].getType() == Node::GROUND)
			{
				voltage->set(i , 0.0);
			}
			else
				assert(0);
		}
	}
	for(int i = 0 ; i < components.size() ; ++i)
	{
		const Component &com = components[i];
		switch(com.getType())
		{
			case Component::VS:
				voltage->set(comIndexToRow[i] , powerGrid->getSupplyVoltage());
				break;
		}
	}
}

void PowerGridSolver::buildMatrixCG()
{
	const vector<Component> &components = powerGrid->getComponents();
	//build matrix G and C
	for(int c = 0 ; c < components.size(); c++)
	{
		const Component &com = components[c];
		Matrix &mg = *matrixG;
		Matrix &mc = *matrixC;
		switch(com.getType())
		{
			case Component::RES:
			{
				mg[com.getVPos()][com.getVPos()] += 1/com.getValue();
				
				if(com.getVNeg() == -1)
					break;
				
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

				if(com.getVNeg() == -1)
					break;
				
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
		}
	}

}

bool PowerGridSolver::solve()
{
	//set current of Nodes
	Vector newVec(int(voltage->size()));
	vector<Node> const &pgNodes = powerGrid->getNodes();
	for(int i = 0 ; i < pgNodes.size() ; i++)
		if(pgNodes[i].getType() == Node::POWER)
			newVec.set(i , - pgNodes[i].getCurrent());
		else if(pgNodes[i].getType() == Node::GROUND)
			newVec.set(i , pgNodes[i].getCurrent());
	
	//set voltage source
	vector<Component> components = powerGrid->getComponents();
	for(int i = 0 ; i < components.size() ; ++i)
	{
		Component &com = components[i];
		switch(com.getType())
		{
			case Component::VS:
				newVec.set(comIndexToRow[i] , com.getValue());
				break;
			
		}
	}
	
    prevPrevVol = prevVol;
	prevVol = voltage;
    for(int i = 0 ; i < prevVol->size() ; i++)
        prevVol->set(i , prevVol->get(i)*2);
	
    
    //build rhs
	*voltage = *matrixC * *voltage;
	*voltage += newVec;

	//solve
	bool success = matrixNs::solve(matrixLHS , voltage);
		
	
	Vector error(matrixC->size());
    for(int i = 0 ; i < voltage->size() ; i++)
        error.set(i , voltage->get(i) - prevVol->get(i) + prevPrevVol->get(i));
    
    for(int i = 0 ; i < error.size() ; i++)
        error.set(i , error.get(i)/timeStep/timeStep);
    Matrix middle(*matrixC);
    middle/=(1/timeStep);
    middle/=(1/timeStep);
    middle/=2;
    error = middle * error;


	matrixNs::solve(matrixLHS , &error);

    //cout<<endl<<"ERROR "<<endl;
    //error.print();

	if(!success){
		return false;
	}
	
	//solve success
	for(int i = 0 ; i < powerGrid->getNodes().size() ; i++){
		powerGrid->getNodes()[i].setVoltage(voltage->get(i));
	}

	return true;
}

