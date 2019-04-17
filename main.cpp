#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <ccomplex>
#include "Longplus.h"
#include "LongPlusPlus.h"
#include "QSMatrix.h"
#include "Polynomial.h"
#include "Eigenvalues.h"

using namespace std;


pair<LongPlusPlus, LongPlusPlus> FibPairLib(int number) 
{
	if (number == 2) {
		return make_pair(LongPlusPlus(1), LongPlusPlus(1));
	}

	if (number == 1) {
		return make_pair(LongPlusPlus(1), LongPlusPlus(0));
	}

	if (number % 2 == 0) {
		auto result = FibPairLib(number / 2);
		auto firstSecondMult = result.first * result.second;
		auto firstSquare = result.first * result.first;
		auto secondSquare = result.second * result.second;
		return make_pair(firstSquare + firstSecondMult + firstSecondMult, firstSquare + secondSquare);
	}
	else {
		auto result = FibPairLib(number - 1);
		return make_pair(result.first + result.second, result.first);
	}
}

// Теоретико-числовой способ
LongPlusPlus FibLib(int number)
{
	auto fibPair = FibPairLib(number);
	return fibPair.first;
}

// Итерационная функция поиска N-го числа фибоначи
LongPlusPlus FibonachiLinear(int n)
{
	LongPlusPlus fibNumbers[2];
	int lastIndex = n % 2;
	fibNumbers[0] = LongPlusPlus(0);
	fibNumbers[1] = LongPlusPlus(1);

	for (int i = 2; i <= n; i++) {
		fibNumbers[i % 2] = fibNumbers[0] + fibNumbers[1];
		lastIndex = i;
	}

	return fibNumbers[lastIndex % 2];
}

// Матричный фибоначи
pair <LongPlusPlus, LongPlusPlus> FibonachiMatrix(int number)
{
	int matrixSize = 2;
	QSMatrix <LongPlusPlus> initMatrix(matrixSize, matrixSize, LongPlusPlus(0));
	QSMatrix <LongPlusPlus> subMatrix(matrixSize, 1, LongPlusPlus(0));
	QSMatrix <LongPlusPlus> proxyMatrix(matrixSize, matrixSize, LongPlusPlus(0));
	initMatrix(0, 0) = LongPlusPlus(1);
	initMatrix(0, 1) = LongPlusPlus(1);
	initMatrix(1, 0) = LongPlusPlus(1);
	initMatrix(1, 1) = LongPlusPlus(0);
	subMatrix(0, 0) = LongPlusPlus(1);
	subMatrix(1, 0) = LongPlusPlus(0);
	proxyMatrix = initMatrix;

	for (int power = 1; power < number; power++) {
		proxyMatrix = proxyMatrix * initMatrix;
	}
	
	auto resultMatrix = proxyMatrix * subMatrix;

	return make_pair(resultMatrix(0, 0), resultMatrix(1, 0));

}

// Тест функций для чисел Фибоначи
void mainTest()
{
	vector<int> random(1000000);
	generate(random.begin(), random.end(), []() {return rand() % 50 + 51; });
	LongPlusPlus sum = LongPlusPlus(0);


	clock_t startTime = clock();
	for (auto r : random) {
		sum = sum + FibonachiLinear(r);
	}
	float resultTime = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	printf("Dynamic. Done in %.2f seconds \n", resultTime);
	printf("Result: %s \n", sum.to_string().c_str());

	sum = LongPlusPlus(0);
	startTime = clock();
	for (auto r : random) {
		sum = sum + FibLib(r);
	}
	resultTime = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	printf("Teoretic. Done in %.2f seconds \n", resultTime);
	printf("Result: %s \n", sum.to_string().c_str());

	sum = LongPlusPlus(0);
	startTime = clock();
	for (auto r : random) {
		sum = sum + FibonachiMatrix(r).second;
	}
	resultTime = (float)(clock() - startTime) / CLOCKS_PER_SEC;
	printf("Matrix. Done in %.2f seconds \n", resultTime);
	printf("Result: %s \n", sum.to_string().c_str());
}

/*
* Печатает матрицу на консоль
*/
template <typename T>
void PrintMatrix(const QSMatrix<T> &matrix)
{
	for (int i = 0; i < matrix.get_rows(); i++) {
		for (int j = 0; j < matrix.get_cols(); j++) {
			cout << matrix(i, j) << " ";
		}
		cout << endl;
	}
}

/*
* Печатает вектор на консоль
*/
template <typename T>
void PrintVector(const vector<T> _vector, bool isVertical = false)
{
	for (int i = 0; i < _vector.size(); i++) {
		cout << _vector[i];

		if (isVertical)
		{
			cout << endl;
		}
		else {
			cout << " ";
		}
	}
	cout << endl;
}


int main()
{
	int matrixSize = 3;
	QSMatrix <complex<double>> matrix(matrixSize, matrixSize, 0);
	matrix(0, 0) = complex<double>(1, 0);
	matrix(0, 1) = complex<double>(2, 0);
	matrix(0, 2) = complex<double>(3, 0);
	matrix(1, 0) = complex<double>(4, 0);
	matrix(1, 1) = complex<double>(5, 0);
	matrix(1, 2) = complex<double>(6, 0);
	matrix(2, 0) = complex<double>(7, 0);
	matrix(2, 1) = complex<double>(8, 0);
	matrix(2, 2) = complex<double>(9, 0);

	
	Eigenvalues eigenValuesInstance;
	Polynomial <complex<double>> eigenPolynomial = eigenValuesInstance.GetEigenPolynomial(matrix);
	cout << "Result" << endl;
	cout << eigenPolynomial << endl;
    cin.get();
	return 0;
}