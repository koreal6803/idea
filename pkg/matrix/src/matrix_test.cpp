#include <iostream>

#include "matrix.h"

using namespace std;
using namespace matrixNs;
int main()
{
	
	int value = 0;
	
	cout << "test Matrix and Vector---" <<endl;
	//build matrix
	Matrix matrixSolver(3);
	for(int row = 0 ; row < 3 ; row++)
		for(int col = 0 ; col < 3 ; col++)
			matrixSolver[row][col] = value++;
	
	//build vector
	Vector v(3);
	for(int row = 0 ; row < 3 ; row++)
		v.set(row, value++);

	matrixSolver.print();
	v.print();
	cout << "\ntest M*V--------------" <<endl<<endl;
	(matrixSolver*v).print();
	cout <<endl;
	cout << "result should be:\n 32 122 212" <<endl;
	cout <<endl;
	
	cout << "test solve MX = V------" <<endl;
	
	solve(&matrixSolver , &v);
	
	for(int i = 0 ; i < v.size() ; i++)
		cout << " " << v.get(i) ;
	cout <<endl <<endl;
	cout << "X should be:\n-2.27 3.8 6.4 " <<endl;

	cout << "---------test add every element in the matrix---------------" <<endl;
	for(int row = 0 ; row < 3 ; row++)
		for(int col = 0 ; col < 3 ; col++)
			matrixSolver[row][col]+=1;
	matrixSolver.print();

	Matrix add1(3);
	Matrix add2(3);
	for(int row = 0 ; row < 3 ; row++)
	{
		for(int col = 0 ; col < 3 ; col++)
		{
			if((row+col)%3 == 1)
				add1[row][col] = 1;
			if((row+col)%3)
				add2[row][col] = 2;
		}
		
	}
	cout << "2matrix adding-----------------------------------------" <<endl;
	add1.print();
	add2.print();
	cout << "add!!!!!!!" <<endl;
	add1+=add2;
	add1.print();
	
	cout << "matrix / operation--------------------------------------" <<endl;
	

	return 0;
}
