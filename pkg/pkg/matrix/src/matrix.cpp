#include <iostream>
#include <assert.h>
#include <iomanip>
#include "matrix.h"

using namespace std;
using namespace matrixNs;
Matrix::Matrix(int n)
{
	symbolic = NULL;
	numeric = NULL;
	
	sizeA = 0;
	sizeAUpdate = false;
	symbolicUpdate = false;
	numericUpdate = false;
	
	Ap = NULL;

	klu_defaults(&common);

	setSizeA(n);
}

Matrix::~Matrix()
{
	if(symbolic)
		klu_free_symbolic(&symbolic, &common);
	
	if(numeric)
		klu_free_numeric(&numeric, &common);
	
	if(Ap)
		delete [] Ap;
}

void Matrix::setSizeA(int n)
{
	assert("you can only set size once!" && !sizeAUpdate);
	sizeA = n;
	sizeAUpdate = true;
	Ap = new int[n+1];
	Ai.assign(n , vector<int>());
	Ax.assign(n , vector<double>());
}

void Matrix::set(int row , int col , double val)
{
	assert("please set size of A first!"&& sizeAUpdate);
	assert("out of boundary!" && row < sizeA && col < sizeA && row >=0 && col >= 0);

	//find out the valk is the new element of sparse matrix or not
	bool isNewElement = true;
	int count = 0;
	assert(col < (int)Ai.size());
	while(count < (int)Ai[col].size())
	{
		assert(col < (int)Ai.size() && count < (int)Ai[col].size());
		if(row == Ai[col][count])
			isNewElement = false;

		if(row <= Ai[col][count])
			break;
		
		count++;
	}
	//nemeric should update since numeric = klu_factor(Ap , Ai , Ax ...);
	numericUpdate = true;
	assert(count <= (int)Ai[col].size());
	if(isNewElement)
	{
		//symbolic should update since symbolic = klu_analyze(Ap , Ai ...);
		symbolicUpdate = true;
		assert(count <= (int)Ai[col].size());
		assert(Ai[col].size() == Ax[col].size());
		Ai[col].insert( Ai[col].begin() + count , row);
		Ax[col].insert( Ax[col].begin() + count , val);
	}
	else{
		assert((int)Ax[col].size() > count);
		Ax[col][count] = val;
	}
}

Proxy Matrix::operator[](int row)
{
	return Proxy(this , row);
}
double &Proxy::operator[](int col)
{
	assert("please set size of A first!"&& m_->sizeAUpdate);
	assert("out of boundary!" && row_ < m_->sizeA && col < m_->sizeA && row_ >=0 && col >= 0);
	
	bool isNewElement = true;
	int count = 0;
	while(count < (int)m_->Ai[col].size())
	{

		if(row_ == m_->Ai[col][count])
			isNewElement = false;

		if(row_ <= m_->Ai[col][count])
			break;

		count++;
	}
	//nemeric should update since numeric = klu_factor(Ap , Ai , Ax ...);
	m_->numericUpdate = true;
	
	if(isNewElement)
	{
		//symbolic should update since symbolic = klu_analyze(Ap , Ai ...);
		m_->symbolicUpdate = true;
		m_->Ai[col].insert( m_->Ai[col].begin() + count , row_);
		m_->Ax[col].insert( m_->Ax[col].begin() + count , 0);
	}

	return m_->Ax[col][count];
/*
	//find out the valk is the new element of sparse matrix or not
	bool isNewElement = true;

	list<int>::iterator AiInsertPos = m_->Ai[col].begin();
	list<double>::iterator AxInsertPos = m_->Ax[col].begin();
	
	while( AiInsertPos != m_->Ai[col].end() && AxInsertPos != m_->Ax[col].end())
	{
		if(*AiInsertPos < row_)
		{
			AiInsertPos++;
			AxInsertPos++;
		}
		else if(*AiInsertPos == row_){//the element already exist
			isNewElement = false;
			break;
		}
		else
			break;
	}
	
	//nemeric should update since numeric = klu_factor(Ap , Ai , Ax ...);
	m_->numericUpdate = true;
	
	list<double>::iterator ret;
	if(isNewElement)
	{
		//symbolic should update since symbolic = klu_analyze(Ap , Ai ...);
		m_->symbolicUpdate = true;
		m_->Ai[col].insert(AiInsertPos , row_);
		m_->Ax[col].insert(AxInsertPos , 0);
		AxInsertPos--;
	}
	*/
}

Vector::Vector(int n)
{
	size_ = n;
	v_ = new double[n];
	for(int i = 0 ; i < size_ ; ++i)
		v_[i] = 0;
}

Vector::Vector(const Vector &v)
{
	size_ = v.size_;
	v_ = new double[size_];
	for(int i = 0 ; i < size_ ; ++i)
		v_[i] = v.v_[i];
}
Vector::~Vector()
{
	delete [] v_;
}

void Vector::print()
{
	cout << "vector^T: "<<endl;
	for(int i = 0 ; i < size_ ; i++)
		cout <<setprecision(10) << v_[i] << "\t" /*<< endl*/;
	cout <<endl;
}

void Vector::operator=(const Vector& v)
{
	delete [] v_;
	size_ = v.size_;
	v_ = new double[size_];
	for(int i = 0 ; i < size_ ; ++i)
		v_[i] = v.v_[i];
}

void Vector::operator+=(const Vector& v)
{
	assert("*ERROR: vector size different cant + " && size_ == v.size_);
	
	for(int i = 0 ; i < size_ ; ++i)
		v_[i] += v.v_[i];
}

bool Vector::set(int row , double val)
{
	if(row >= size_){
		cout << "*WARN: out of range " <<endl;
		return false;
	}
	assert(row < size_);
	v_[row] = val;
	return true;
}

double Vector::get(int row)
{
	if(row >= size_)
		return -1;
	return v_[row];
}

Vector Matrix::operator*(Vector &v)
{
	assert("matrix multiply fail due to incompatable size!" && v.size() == size());
	
	Vector retVec2(size());
	for(int i = 0 ; i < size() ; ++i)
		for(unsigned j = 0 ; j < Ai[i].size() ; j++)
			retVec2.set(Ai[i][j] , retVec2.get(Ai[i][j]) + Ax[i][j]*v.get(i));

	return retVec2;
}

bool matrixNs::solve(Matrix *a , Vector *v)
{
	assert("*ERROR: size of matrix and vector are not same! cant solave!" && a->size() == v->size());
	a->updateSymbolicAndNumeric();

	if(!klu_solve(a->symbolic , a->numeric , a->size() , 1 , v->v_ , &a->common))
	{
		cout << "\n\n*WARRN klu solve fail!\n\n" <<endl;
		return false;
	}
	return true;
}

void Matrix::updateSymbolicAndNumeric()
{
	int *AiTemp = NULL;
	double *AxTemp = NULL;
	
	if(symbolicUpdate)
	{
		//build Ap
		Ap[0] = 0;
		int cnt(1);
		for(vector<vector<int> >::iterator it = Ai.begin() ; it != Ai.end() ; it++)
		{
			Ap[cnt] = it->size() + Ap[cnt-1];
			cnt++;
		}

		//build Ai
		AiTemp = new int[Ap[sizeA]];
		cnt = 0;
		for(vector<vector<int> >::iterator it = Ai.begin() ; it != Ai.end() ; it++)
			for(vector<int>::iterator jt = it->begin() ; jt != it->end() ; jt++)
				AiTemp[cnt++] = *jt;
		//update symbolic
		symbolic = klu_analyze(sizeA , Ap , AiTemp , &common);
	}

	if(numericUpdate)
	{
		//build Ax
		int cnt(0);
		AxTemp = new double[Ap[sizeA]];
		for(vector<vector<double> >::iterator it = Ax.begin() ; it != Ax.end() ; it++)
			for(vector<double>::iterator jt = it->begin() ; jt != it->end() ; jt++)
				AxTemp[cnt++] = *jt;
	
		//updatre numeric
		numeric = klu_factor(Ap , AiTemp , AxTemp , symbolic , &common);
	}
	
	if(AiTemp)
		delete [] AiTemp;
	if(AxTemp)
		delete [] AxTemp;

	numericUpdate = false;
	symbolicUpdate = false;
}

void Matrix::operator+=(const Matrix &m)
{
	assert(m.sizeA == sizeA);
	vector<int> rowPos(m.sizeA , 0);
	//cout << "start" <<endl;
	for(int i = 0 ; i < m.sizeA ; ++i)
	{
		//cout << "for start" <<endl;
		int selfCnt(0);
		int mCnt(0);
		assert(Ai.size());
		while(selfCnt <(int) Ai[i].size() && mCnt < (int)m.Ai[i].size())
		{
			//cout << "------------------------" <<endl;
			//cout << "self: " << selfCnt <<endl;
			//cout << "mCnt: " << mCnt <<endl;
			if(Ai[i][selfCnt] == m.Ai[i][mCnt])
			{
				//cout << "case 1" <<endl;
				Ax[i][selfCnt]+=m.Ax[i][mCnt];
				mCnt++;
				selfCnt++;
			}
			else if(Ai[i][selfCnt] < m.Ai[i][mCnt])
			{
				//cout << "case 2" <<endl;
				selfCnt++;
			}
			else if(Ai[i][selfCnt] > m.Ai[i][mCnt])
			{
				//cout << "case 3" <<endl;
				Ai[i].insert(Ai[i].begin() + selfCnt , m.Ai[i][mCnt]);
				Ax[i].insert(Ax[i].begin() + selfCnt , m.Ax[i][mCnt]);
				selfCnt++;
				mCnt++;
			}
		}
		//cout << "compare end" <<endl;
		for(;mCnt < (int)m.Ai[i].size();mCnt++)
		{
			Ai[i].insert(Ai[i].begin() + selfCnt , m.Ai[i][mCnt]);
			Ax[i].insert(Ax[i].begin() + selfCnt , m.Ax[i][mCnt]);
			selfCnt++;
		}		
	}
}


void Matrix::operator-=(const Matrix &m)
{
	assert(m.sizeA == sizeA);
	vector<int> rowPos(m.sizeA , 0);
	//cout << "start" <<endl;
	for(int i = 0 ; i < m.sizeA ; ++i)
	{
		//cout << "for start" <<endl;
		int selfCnt(0);
		int mCnt(0);
		assert(Ai.size());
		while(selfCnt < (int)Ai[i].size() && mCnt < (int)m.Ai[i].size())
		{
			//cout << "------------------------" <<endl;
			//cout << "self: " << selfCnt <<endl;
			//cout << "mCnt: " << mCnt <<endl;
			if(Ai[i][selfCnt] == m.Ai[i][mCnt])
			{
				//cout << "case 1" <<endl;
				Ax[i][selfCnt]-=m.Ax[i][mCnt];
				mCnt++;
				selfCnt++;
			}
			else if(Ai[i][selfCnt] < m.Ai[i][mCnt])
			{
				//cout << "case 2" <<endl;
				selfCnt++;
			}
			else if(Ai[i][selfCnt] > m.Ai[i][mCnt])
			{
				//cout << "case 3" <<endl;
				Ai[i].insert(Ai[i].begin() + selfCnt , -m.Ai[i][mCnt]);
				Ax[i].insert(Ax[i].begin() + selfCnt , -m.Ax[i][mCnt]);
				selfCnt++;
				mCnt++;
			}
		}
		//cout << "compare end" <<endl;
		for(;mCnt < (int)m.Ai[i].size();mCnt++)
		{
			Ai[i].insert(Ai[i].begin() + selfCnt , -m.Ai[i][mCnt]);
			Ax[i].insert(Ax[i].begin() + selfCnt , -m.Ax[i][mCnt]);
			selfCnt++;
		}		
	}
}

void Matrix::operator/=(double h)
{
	for(unsigned i = 0 ; i < Ax.size() ; i++)
		for(unsigned j = 0 ;j < Ax[i].size() ;j++)
		{
			Ax[i][j] /=h;
		}
}

Matrix::Matrix(const Matrix &m)
{
	symbolic = NULL;
	numeric = NULL;
	
	sizeA = m.sizeA;
	sizeAUpdate = false;
	symbolicUpdate = false;
	numericUpdate = false;
	
	Ap = NULL;

	klu_defaults(&common);


	assert(sizeA == m.sizeA);
	setSizeA(m.sizeA);
	assert((int)Ai.size() == m.sizeA);
	for(unsigned i = 0 ; i < m.Ai.size() ; i++){
		assert(m.Ai[i].size() == m.Ax[i].size());
		for(unsigned j = 0 ;j < m.Ai[i].size() ;j++){
			int row = m.Ai[i][j];
			int col = i;
			double val = m.Ax[i][j];
			(*this)[row][col] = val;
		}
	}

	assert((int)Ai.size() == m.sizeA);
}

void Matrix::print()
{
	cout << "matrix^T:" <<endl;
	for(unsigned i = 0; i < Ax.size() ; i++)
	{
		int counter(0);
		
		vector<int>::iterator AiPos = Ai[i].begin();
		vector<double>::iterator AxPos = Ax[i].begin();
		
		while( AiPos != Ai[i].end() && AxPos != Ax[i].end())
		{
			if(*AiPos > counter)
			{
				counter++;
				cout << setw(3) << "0";
			}
			else if(*AiPos == counter)
			{
				if(*AxPos >= 0)
					cout << setw(3)<<setprecision(2) << *AxPos;
				else
					cout << setw(3)<<setprecision(2) << *AxPos;
				counter++;
				AiPos++;
				AxPos++;
			}
			else
				assert(0);
		}
		while(counter < (int)Ax.size())
		{
			cout << setw(3) << "0";
			counter++;
		}
		cout << endl;
	}
	cout <<endl;
}

