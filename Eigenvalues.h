#pragma once

#include <vector>
#include <complex>
#include <random>

using namespace std;

template <typename T>
class Eigenvalues
{
	QSMatrix <T> matrixInstance;
public:
	Eigenvalues(const QSMatrix <T> &inputMatrix) 
	{
		this->matrixInstance = inputMatrix;
	}

	QSMatrix <T> GetSubmatrix()
	{
		QSMatrix <T> stubMatrix(0, 0, 0);
		int matrixRows = this->matrixInstance.get_rows();
		int matrixCols = this->matrixInstance.get_cols();

		int subMatrixRows = matrixRows - 1;
		int subMatrixCols = matrixCols - 1;

		if (subMatrixRows < 0 || subMatrixCols < 0)
		{
			return stubMatrix;
		}

		QSMatrix <T> subMatrix(subMatrixRows, subMatrixCols, 0);
		
		for (int i = 1; i < matrixRows; i++) {
			for (int j = 1; j < matrixCols; j++) {
				subMatrix(i, j) = this->matrixInstance(i, j);
			}
		}

		return subMatrix;
	}

};