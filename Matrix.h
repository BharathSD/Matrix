#pragma once
#include <iostream>
#include "Exception.h"

template<class DataType>
class Matrix
{
protected:
	int m_nCols;

	int m_nRows;

	DataType *m_pData;
public:
	Matrix():m_nRows(0), m_nCols(0),  m_pData(NULL)
	{
	}
	~Matrix()
	{
		if(this->m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
	}
	Matrix(Matrix &&Ob)
	{
		this->m_nRows = Ob.m_nRows;
		this->m_nCols = Ob.m_nCols;
		this->m_pData = Ob.m_pData;
		Ob.m_pData = NULL;
	}

	Matrix(Matrix &Ob)
	{
		this->m_nRows = Ob.m_nRows;
		this->m_nCols = Ob.m_nCols;
		int nTotalElems(this->m_nRows * this->m_nCols);

		this->m_pData = new DataType[nTotalElems];
		std::copy(Ob.m_pData, Ob.m_pData +  nTotalElems, this->m_pData);
	}

	Matrix(int nDimension): m_nRows(nDimension), m_nCols(nDimension), m_pData(NULL)
	{
		m_pData = new DataType[m_nRows * m_nCols];
	}

	Matrix(int nDimensions, DataType *pData): m_nRows(nDimensions), m_nCols(nDimensions), m_pData(pData)
	{
	}

	Matrix(int nRows, int nCols):m_nRows(nRows), m_nCols(nCols)
	{
		m_pData = new DataType[m_nRows * m_nCols];
	}


	Matrix(int nRows, int nCols, DataType *pData): m_nRows(nRows), m_nCols(nCols)
	{
		int nElems( m_nRows * m_nCols);
		m_pData = new DataType[nElems];
		std::copy(pData, pData + nElems, m_pData);
	}

	Matrix& operator=(Matrix Ob)
	{
		if(this != &Ob)
		{
			this->m_nRows = Ob.m_nRows;
			this->m_nCols = Ob.m_nCols;
			
			std::swap(this->m_pData, Ob.m_pData);
		}
		return *this;
	}

	bool SetData(DataType *pData, int DataElems)
	{
		bool bIsValid = (DataElems == (m_nRows * m_nCols));
		if(bIsValid)
		{
			m_pData = new int[DataElems];
			memcpy(m_pData, pData, DataElems);
		}
		return bIsValid;
	}

	DataType& operator() (int nRow, int nCol)
	{
		return this->m_pData[(nRow * this->m_nCols) + nCol];
	}

	DataType& operator[] (int nIndex)
	{
		return this->m_pData[nIndex];
	}

	void Add(Matrix &Ob1)
	{
		bool IsValid = ((this->m_nRows == Ob1.m_nRows) && (this->m_nCols == Ob1.m_nCols));
		if(!IsValid)
		{
			throw InValidData();
		}
		/*Matrix Result(this->m_nRows, this->m_nCols, this->m_pData);*/
		int nElems = this->m_nRows * m_nCols;
		for(int nItr(0); nItr < nElems; ++nItr)
		{
			this->m_pData[nItr] += Ob1.m_pData[nItr];
		}
	}

	void Sub(Matrix &Ob1)
	{
		bool IsValid = ((this->m_nRows == Ob1.m_nRows) && (this->m_nCols == Ob1.m_nCols));
		if(!IsValid)
		{
			throw InValidData();
		}
		/*Matrix Result(this->m_nRows, this->m_nCols, this->m_pData);*/
		int nElems = m_nRows * m_nCols;
		for(int nItr(0); nItr < nElems; ++nItr)
		{
			this->m_pData[nItr] -= Ob1.m_pData[nItr];
		}
	}

	void Mul(Matrix &Ob1)
	{
		bool bIsValid = this->m_nCols == Ob1.m_nRows;
		if(!bIsValid)
		{
			throw MatrixMulError();
		}
		Matrix<DataType> Result(this->m_nRows, Ob1.m_nCols);

		for(int nResRowItr(0); nResRowItr < Result.m_nRows; ++nResRowItr)
		{
			for(int nResColItr(0); nResColItr < Result.m_nCols; ++nResColItr)
			{
				int nSum(0);
				for(int nItr(0); nItr < this->m_nCols; ++nItr)
				{
					nSum += this->operator()(nResRowItr,nItr) * Ob1.operator()(nItr, nResColItr);
				}
				Result.operator()(nResRowItr, nResColItr) = nSum;
			}
		}
		*this =  Result;
	}

	void Mul(int nScalarVal)
	{
		bool bIsValid = ((this->m_nRows > 0) && (this->m_nCols > 0));

		if(!bIsValid)
		{
			throw InValidData();
		}
		int nElems = m_nRows * m_nCols;
		for(int nItr(0); nItr < nElems; ++nItr)
		{
			this->m_pData[nItr] *= nScalarVal;
		}
	}

	void Transpose()
	{
		bool bIsValid = ((this->m_nRows > 0) && (this->m_nCols > 0));

		if(!bIsValid)
		{
			throw InValidData();
		}
		Matrix Result(this->m_nCols, this->m_nRows);
		for(int nRowItr(0); nRowItr < m_nRows; nRowItr++)
		{
			for(int nColItr(0); nColItr < m_nCols; nColItr++)
			{
				Result.operator()(nColItr, nRowItr) = this->operator()(nRowItr, nColItr);
			}
		}

		*this = Result;
	}

	Matrix<float> Inverse()
	{
		/*bool bIsValid = this->m_nRows == this->m_nCols;

		if(!bIsValid)
		{
			throw MatrixInvError();
		}

		Matrix<float> Result(this->m_nRows);

		Result.SetIdentity();
		int i,j,k,L;
		float t;
		for(j = 0; j < this->m_nCols; ++j)
		{
			for(i = 0; i < this->m_nRows; ++i)
			{
				if(this->operator()(i,j) != 0)
				{
					for(k = 0; k < this->m_nRows; ++k)
					{
						std::swap(this->operator()(j,k), this->operator()(i,k));
						std::swap(Result.operator()(j,k),Result.operator()(i,k));
					}
					float t = 1.0f/this->operator()(j,j);

					for(k = 0; k < this->m_nRows; ++k)
					{
						this->operator()(j,k) = t * this->operator()(j, k);
						Result.operator()(j,k) = t * Result.operator()(j, k);
					}

					for(L = 0; L < this->m_nRows; ++L)
					{
						if( L != j)
						{
							t = -this->operator()(L,j);
							for(int k(0); k < this->m_nRows; ++k)
							{
								this->operator()(L,k) = this->operator()(L,k) + t * this->operator()(j,k);
								Result.operator()(L,k) = Result.operator()(L,k) + t * Result.operator()(j,k);
							}
						}
					}

				}
			}
			break;
		}*/
		cout << "To be Done" << endl;
	}

	void SetIdentity()
	{
		bool bIsValid = this->m_nRows == this->m_nCols;

		if(!bIsValid)
		{
			throw IdentityMatError();
		}

		for(int nItr(0); nItr < this->m_nRows; ++nItr)
		{
			this->m_pData[(nItr * this->m_nCols) + nItr] = (DataType)1;
		}
	}
	

	Matrix operator+(Matrix &Ob)
	{
		Matrix<DataType> Result(*this);
		Result.Add(Ob);
		return Result;
	}
	
	Matrix operator-(Matrix &Ob)
	{
		Matrix<DataType> Result(*this);
		Result.Sub(Ob);
		return Result;
	}

	Matrix operator*(Matrix &Ob)
	{
		Matrix<DataType> Result(*this);
		Result.Mul(Ob);
		return Result;
	}
	
	Matrix& operator+=(Matrix &Ob)
	{
		this->Add(Ob);
		return *this;
	}
	
	Matrix& operator-=(Matrix &Ob)
	{
		this->Sub(Ob);
		return *this;
	}

	Matrix& operator*=(Matrix &Ob)
	{
		this->Mul(Ob);
		return *this;
	}

	friend ostream &operator<<( ostream &output, Matrix & Ob) 
	{ 
			for(int nRowItr(0); nRowItr <Ob.m_nRows; ++nRowItr)
			{
				for(int nColItr(0); nColItr < Ob.m_nCols; ++nColItr)
				{
					output << Ob(nRowItr, nColItr) << "\t";
				}
				output << endl;
			}
			return output;            
	}
};