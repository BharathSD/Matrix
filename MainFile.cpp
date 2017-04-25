#include <iostream>
#include "Matrix.h"
using namespace std;

int main()
{
	int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9};
	Matrix<int> Mat1(3,3,a);
	Matrix<int> Mat2(Mat1);
	Mat2.Transpose();
	Mat1(2,2);

	cout << Mat1 << endl;
	cout << Mat2 << endl;

	Mat1.Add(Mat2);
	cout << Mat1 << endl;
	cout << Mat2 << endl;

	Matrix<int> Result = Mat1 + Mat2;
	cout << Result << endl;

	Result = Mat1 - Mat2;
	cout << Result << endl;

	Result += Mat2;
	cout << Result << endl;

	/*Mat1.Mul(Mat2);
	cout << Mat1 << endl;*/
	/*Matrix<float> Res = Mat1.Inverse();
	cout << Res << endl;*/
	return EXIT_SUCCESS;
}