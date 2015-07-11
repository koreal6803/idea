#ifndef __MATRIX_MATRIX_H__
#define __MATRIX_MATRIX_H__
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include "klu.h"

namespace matrixNs{

class Vector;
class Matrix;

bool solve(Matrix *a , Vector *v);

class Vector
{
	friend bool matrixNs::solve(Matrix *a , Vector *v);
	
	public:
		Vector(int n);
		Vector(const Vector&);
		~Vector();
		
		//dont use it, otherwise compile will fail!!
		//dont implement it or i will kill you!!!
		Vector();

		bool set(int row , double val);
		bool add(int row , double val);
		double get(int row);
		int size(){return size_;}
		void print();
	
    	void operator=(const Vector&);
		void operator+=(const Vector&);
	private:
	
		int size_;
		double *v_;
};



struct Proxy;
class Matrix
{
	friend bool matrixNs::solve(Matrix *a , Vector *v);
	public:
		
		//solve A x = B
		Matrix(int n);

		~Matrix();

		//build nxn matrix
		//( we shold use this command before setA and setB )
		
		//set A x = B
		void set(int row , int col , double val);
		Proxy operator[](int row);
		
		//solve
		std::vector<double> luSolve();

		
		//print
		void print();
		int size(){return sizeA;}

		Vector operator*(Vector &v);
		void operator+=(const Matrix &m);
        void operator-=(const Matrix &m);
		void operator/=(double h);
		Matrix(const Matrix &m);
	private:
		
    	Matrix& operator=(const Matrix&);//non-copyable
		
		void setSizeA(int n);
		
		void updateSymbolicAndNumeric();

		int sizeA;

		//klu basic element
		klu_symbolic *symbolic;
		klu_numeric *numeric;
		klu_common common;

		//flag

		bool sizeAUpdate;
		bool symbolicUpdate;
		bool numericUpdate;
		
		//Ap:
		int *Ap;

		//Ai:
		std::vector<std::vector<int> > Ai;

		//Ax:
		std::vector<std::vector<double> > Ax;

		
	friend class Proxy;
};
struct Proxy
{
	Matrix *m_;
	int row_;
	Proxy(Matrix *matrix , int row):m_(matrix),row_(row){}
	double &operator[](int col);
};

};

#endif

