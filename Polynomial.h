#pragma once
#include <vector>
#include <complex>
#include <random>

using namespace std;

template <typename T>
class Polynomial
{
private:
	vector<T> coefficients;
public:
	Polynomial(const vector<T> &coefficients)
	{
		this->coefficients = coefficients;
	}

	Polynomial(const int &polynomialDegree)
	{
		this->coefficients.resize(polynomialDegree + 1);
	}

	vector <T> Coefficients() const
	{
		return this->coefficients;
	}
	
	/*
	* - Возвращает степень многочлена P(x)
	*/
	int Degree() const
	{
		return this->coefficients.size() - 1;
	}

	const T& operator [](const int &n) const
	{
		return this->coefficients[n];
	}

	T& operator [](const int &n)
	{
		return this->coefficients[n];
	}

	/*
	* Вычисление значения многочлена P(x)
	* @param int value - значение x
	*/
	T operator ()(const T &value)
	{
		T result = 0;
		for (int i = 0; i < this->coefficients.size(); i++) {
			result += this->coefficients[i] * pow(value, i);
		}
		return result;
	}

	/*
	* Деленит многочлен P(x) на x - a
	* @param T coefficient - коэффициент a
	* @return Polynomial<T> - нормализованный многочлен
	*/
	Polynomial<T> Divide(const T &coefficient)
	{
		int currentPolyDegree = this->Degree();
		Polynomial<T> result(currentPolyDegree - 1);
		result.coefficients.back() = this->coefficients.back();
		for (int i = currentPolyDegree - 2; i >= 0; i--) {
			result[i] = coefficient * result[i + 1] + this->coefficients[i + 1];
		}

		return result;
	}

	/*
	* Нормализует многочлен путём деления его на коэффициент
	* @param T coefficient - коэффициент
	* @return Polynomial<T> - нормализованный многочлен
	*/
	Polynomial<T> Normalize(const T &coefficient)
	{
		int currentPolyDegree = this->Degree();
		Polynomial<T> result(currentPolyDegree);
		for (int i = 0; i < this->coefficients.size(); i++) {
			result[i] = this->coefficients[i] / coefficient;
		}

		return result;
	}

	/*
	* Находит производную многочлена
	* @return Polynomial<T> - производная многочлена
	*/
	Polynomial<T> Derivative()
	{
		int currentPolyDegree = this->Degree();
		Polynomial<T> result(currentPolyDegree - 1);
		for (int i = currentPolyDegree; i > 0; i--) {
			result[i - 1] = this->coefficients[i] * i;
		}
		return result;
	}

	QSMatrix <complex<double>> GeneratePolynomialComplexMatrix(const Polynomial<complex<double>> polynomial)
	{
		int matrixSize = polynomial.Coefficients().size() - 1;
		int countRow = matrixSize;
		int countCol = matrixSize;
		int startCoeffIndex = matrixSize - 1;
		QSMatrix <complex<double>> polyMatrix(countRow, countCol, 0);
		for (int i = startCoeffIndex; i >= 0; i--) {
			polyMatrix(0, startCoeffIndex - i) = -polynomial[i];
		}

		for (int i = 0; i < polyMatrix.get_rows(); i++) {
			for (int j = 0; j < polyMatrix.get_cols(); j++) {
				if (i == j && (i + 1) < polyMatrix.get_rows()) {
					polyMatrix(i + 1, j) = 1;
				}
			}
		}

		return polyMatrix;
	}

	/*
	* Возвращает вектор случайных комплексных чисел
	* @param int vectorSize - размер вектора
	*/
	QSMatrix <complex<double>> GenerateRandomComplexVector(int vectorSize)
	{
		QSMatrix <complex<double>> result(vectorSize, 1, 0);
		for (int i = 0; i < vectorSize; i++) {
			result(i, 0) = this->GetRandomComplexNumber();
		}
		return result;
	}

	/*
	* Печатает матрицу на консоль
	*/
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
	void PrintVectorVertical(const vector<T> &_vector)
	{
		for (int i = 0; i < _vector.size(); i++) {
			cout << _vector[i] << endl;
		}
	}

	void FindComplexRoot()
	{
		// Нормализуем многочлен
		// Делим все коэффициенты на коэфф. при старшей степени
		T lastCoeff = this->coefficients[this->coefficients.size() - 1];
		Polynomial<T> normalizePoly = this->Normalize(lastCoeff);
		
		// Генерируем случайное Альфа
		complex <double> randomAlpha = this->GetRandomComplexNumber();
		// Сдвигаем многочлен на Альфа
		Polynomial<complex<double>> shiftedPoly = normalizePoly.Shift(randomAlpha);
		// Генерируем матрицу
		QSMatrix <complex<double>> polyMatrix = this->GeneratePolynomialComplexMatrix(shiftedPoly);
		QSMatrix <complex<double>> randomVector = this->GenerateRandomComplexVector(polyMatrix.get_cols());

		// Умножение матриц по формуле
		QSMatrix <complex<double>> result = polyMatrix * randomVector;

		PrintMatrix(polyMatrix);
		cout << endl;
		PrintMatrix(randomVector);
		cout << endl;
		PrintMatrix(result);
	}

	/*
	* Возвращает случайное комплексное число
	* @return complex<double> - комплексное число
	*/
	complex<double> GetRandomComplexNumber()
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(-0.5, 0.5);
		double real = dis(gen);
		double imag = dis(gen);
		return complex <double>(real, imag);
	}

	/*
	* Сдвиг многочлена на коэффициент a. P(x) -> P(x + a)
	* @param T coefficient - коэффициент a
	* @return Polynomial<T> - нормализованный многочлен
	*/
	Polynomial<T> Shift(T coefficient)
	{
		int currentPolyDegree = this->Degree();
		vector<T> resultCoefficients;
		for (int i = 0; i < currentPolyDegree; i++) {
			// Делим многочлен на x-a
			auto tempPoly = this->Divide(coefficient);
			// Берём остаток от деления
			auto remainder = tempPoly(coefficient);
			// Сохраняем его
			resultCoefficients.push_back(remainder);
		}

		// Сохраняем последнюю степень
		resultCoefficients.push_back(this->coefficients[this->coefficients.size() - 1]);
		return Polynomial <T>(resultCoefficients);
	}

	friend ostream& operator<< (ostream &out, const Polynomial <T> &rhs) {

		int coeffSize = rhs.coefficients.size();
		for (int i = coeffSize - 1; i >= 0; i--) {
			out << rhs.coefficients[i];
			if (i > 0) {
				out << "*" << "(x^" << i << ") ";
			}
		}
		out << endl;
		return out;
	}


};

