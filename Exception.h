#pragma once
#include <iostream>
#include <exception>

using namespace std;

class InValidData : public exception
{
public:
	const char * what() const throw()
	{
		return "Invalid Data\n";
	}
};

class MatrixMulError : public exception
{
public:
	const char * what() const throw()
	{
		return "Matrix Multiplication not possible\n";
	}
};

class MatrixInvError : public exception
{
public:
	const char * what() const throw()
	{
		return "Matrix Inversion not possible\n";
	}
};

class IdentityMatError : public exception
{
public:
	const char * what() const throw()
	{
		return "Identity Matrix not possible\n";
	}
};