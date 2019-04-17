#pragma once

#include <vector>
#include <complex>
#include <random>
#include <assert.h>

using namespace std;

class Eigenvalues
{
public:
	// @see https://en.wikipedia.org/wiki/Samuelson%E2%80%93Berkowitz_algorithm
	Eigenvalues() {}

	// Возвращает подматрицу размера (n - 1) x (n - 1)
	// Матрица с вычеркиванием первой строки и столбца
	QSMatrix <complex<double>> GetSubmatrix(const QSMatrix <complex<double>> &matrixInstance)
	{
		QSMatrix <complex<double>> stubMatrix(0, 0, 0);
		int matrixRows = matrixInstance.get_rows();
		int matrixCols = matrixInstance.get_cols();

		int subMatrixRows = matrixRows - 1;
		int subMatrixCols = matrixCols - 1;

		if (subMatrixRows < 0 || subMatrixCols < 0)
		{
			return stubMatrix;
		}

		QSMatrix <complex<double>> subMatrix(subMatrixRows, subMatrixCols, 0);
		
		for (int i = 1; i < matrixRows; i++) {
			for (int j = 1; j < matrixCols; j++) {
				subMatrix(i - 1, j - 1) = matrixInstance(i, j);
			}
		}

		return subMatrix;
	}

	// Возвращает R вектор
	vector <complex<double>> GetRVector(const QSMatrix <complex<double>> &matrixInstance)
	{
		int matrixRows = matrixInstance.get_rows();
		vector <complex<double>> rVector;

		for (int i = 1; i < matrixRows; i++) {
			rVector.push_back(matrixInstance(0, i));
		}

		return rVector;
	}

	// Возвращает C вектор
	vector <complex<double>> GetCVector(const QSMatrix <complex<double>> &matrixInstance)
	{
		int matrixCols = matrixInstance.get_cols();
		vector <complex<double>> cVector;

		for (int i = 1; i < matrixCols; i++) {
			cVector.push_back(matrixInstance(i, 0));
		}

		return cVector;
	}

	// Возводит матрицу в степень
	QSMatrix <complex<double>> GetMatrixPow(const QSMatrix <complex<double>> &matrix, int matrixDegree)
	{
		QSMatrix <complex<double>> powMatrix = matrix;
		for (int i = 1; i < matrixDegree; i++) {
			powMatrix = powMatrix * matrix;
		}

		return powMatrix;
	}

	// Возвращает произведение двух векторов
	complex<double> VectorComposition(const vector<complex<double>> &a, const vector<complex<double>> &b)
	{
		int aSize = a.size();
		int bSize = b.size();
		complex<double> result(0, 0);

		if (aSize != bSize) {
			assert("Vectors sizes are not equal");
		}

		for (int i = 0; i < aSize; i++) {
			result += a[i] * b[i];
		}

		return result;
	}

	// Умножает вектор на матрицу
	vector<complex<double>> MultiVectorMatrix(const vector<complex<double>> &rVector, const QSMatrix <complex<double>> &lMatrix)
	{
		int vectorSize = rVector.size();
		int matrixRows = lMatrix.get_rows();
		int matrixCols = lMatrix.get_cols();

		if (vectorSize != matrixRows) {
			assert("Vector size doesn't match matrix count rows");
		}

		vector<complex<double>> result;

		for (int i = 0; i < matrixCols; i++)
		{
			complex<double> temp(0, 0);
			for (int j = 0; j < matrixRows; j++)
			{
				temp += rVector[j] * lMatrix(j, i);
			}
			result.push_back(temp);
		}

		return result;
	}

	// Возвращает значение на диагонали
	complex<double> GetDiagonalComposition(const vector<complex<double>> &rVector, const vector<complex<double>> &cVector, const QSMatrix <complex<double>> &subMatrix, int subMatrixDegree)
	{
		if (subMatrixDegree == 0) {
			return -VectorComposition(rVector, cVector);
		}

		complex<double> result(0, 0);
		QSMatrix <complex<double>> powMatrix = this->GetMatrixPow(subMatrix, subMatrixDegree);
		result = this->VectorComposition(this->MultiVectorMatrix(rVector, powMatrix), cVector);
		return -result;
	}

	// Возвращает Т матрицу для матрицы
	QSMatrix <complex<double>> GetTSubMatrix(const QSMatrix <complex<double>> &matrixInstance)
	{
		int matrixRows = matrixInstance.get_rows();
		int matrixCols = matrixInstance.get_cols();

		if (matrixRows != matrixCols) {
			assert("Matrix size should be a square");
		}

		if (matrixRows == 0 && matrixCols == 0) {
			assert("Empty matrix size");
		}

		// Т.к квадратная матрица
		int matrixSize = matrixRows;

		int tMatrixRows = matrixSize + 1;
		int tMatrixCols = matrixSize;
		complex<double> tMatrixFirstElement = -matrixInstance(0, 0);
		QSMatrix <complex<double>> tMatrix(tMatrixRows, tMatrixCols, 0);

		if (matrixSize == 1)
		{
			tMatrix(0, 0) = 1;
			tMatrix(1, 0) = tMatrixFirstElement;
			return tMatrix;
		}

		QSMatrix <complex<double>> subMatrix = this->GetSubmatrix(matrixInstance);
		vector <complex<double>> rVector = this->GetRVector(matrixInstance);
		vector <complex<double>> cVector = this->GetCVector(matrixInstance);

		for (int i = 0; i < tMatrixRows; i++) {
			for (int j = 0; j < tMatrixCols; j++) {
				// Вне главной диагонали нули
				if (i < j) {
					tMatrix(i, j) = 0;
					continue;
				}

				// На главной диагонали единицы
				if (i == j) {
					tMatrix(i, j) = 1;
					continue;
				}

				// На первой поддиагонали коэфф
				if (i - 1 == j || j + 1 == i) {
					tMatrix(i, j) = tMatrixFirstElement;
					continue;
				}

				// На остальных поддиагоналях -R*A1^(k-2)*C
				int kDiagonal = (i - j) - 2;
				tMatrix(i, j) = this->GetDiagonalComposition(rVector, cVector, subMatrix, kDiagonal);
			}
		}

		return tMatrix;
	}

	// Возвращает характеристический многочлен для матрицы
	Polynomial <complex<double>> GetEigenPolynomial(const QSMatrix <complex<double>> &matrix)
	{
		QSMatrix <complex<double>> processingMatrix = matrix;
		vector <QSMatrix<complex<double>>> tMatrixes;

		while (processingMatrix.get_rows() > 1 && processingMatrix.get_cols() > 1)
		{
			QSMatrix <complex<double>> tMatrix = this->GetTSubMatrix(processingMatrix);
			processingMatrix = this->GetSubmatrix(processingMatrix);
			tMatrixes.push_back(tMatrix);
		}

		QSMatrix <complex<double>> tMatrix = this->GetTSubMatrix(processingMatrix);
		tMatrixes.push_back(tMatrix);

		QSMatrix <complex<double>> coeffMatrix = tMatrixes[0];
		vector <complex<double>> coeffVector;

		for (int i = 1; i < tMatrixes.size(); i++) {
			coeffMatrix = coeffMatrix * tMatrixes[i];
		}

		for (int i = coeffMatrix.get_rows() - 1; i >= 0; i--) {
			coeffVector.push_back(coeffMatrix(i, 0));
		}
		
		return Polynomial <complex<double>> (coeffVector);
	}
};