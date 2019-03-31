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

	void FindRoot()
	{
		// Нормализуем многочлен
		// Делим все коэффициенты на коэфф. при старшей степени
		T lastCoeff = this->coefficients[this->coefficients.size() - 1];
		Polynomial<T> normalizePoly = this->Normalize(lastCoeff);


	}


	/*
	* Сдвиг многочлена на коэффициент a. P(x) -> P(x + a)
	* @param T coefficient - коэффициент a
	* @return Polynomial<T> - нормализованный многочлен
	*/
	Polynomial<T> Shift(const T& coefficient)
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

