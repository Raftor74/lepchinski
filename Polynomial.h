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
			result[i - 1] = this->coefficients[i] * (T)i;
		}
		return result;
	}

	/*
	* Генерирует матрицу для полинома, состоящую из комплексных чисел
	* @param Polynomial<complex<double>> - полином с комплексными коэффициентами
	* @return QSMatrix <complex<double>> - матрица для полинома
	*/
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

	/*
	** Метод Ньютона
	*/
	T Neuton(complex<double> someRoot) {
		auto derivativePolinomial = this->Derivative();
		return someRoot - (*this)(someRoot) / derivativePolinomial(someRoot);
	}

	complex <double> FindComplexRoot()
	{
		int polyDegree = this->Degree();

		if (polyDegree == 1)
		{
			return -this->coefficients[0] / this->coefficients[1];
		}

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

		QSMatrix <complex<double>> squaredMatrix = polyMatrix;
		complex<double> lambda;
		complex<double> uN;
		complex<double> vN;
		complex<double> prevLambda(0, 0);

		int count = 0;
		double difference = 9999;
		double eps = 1e-3;

		// Уточняем начальное лямбда
		while (difference > eps)
		{
			squaredMatrix = squaredMatrix * polyMatrix;
			QSMatrix <complex<double>> resultMatrix = squaredMatrix * randomVector;
			
			uN = resultMatrix(0, 0);
			vN = resultMatrix(1, 0);
			lambda = uN / vN;

			if (count == 0)
			{
				prevLambda = lambda;
			}
			else
			{
				difference = abs(lambda - prevLambda);
				prevLambda = lambda;
			}

			count++;
		}

		// Уточняем лямбда с помощью метода ньютона
		count = 0;
		difference = 9999;
		eps = 1e-10;
		complex <double> initRoot = lambda;

		while (difference > eps)
		{
			auto nextRoot = this->Neuton(initRoot);
			difference = abs(initRoot - nextRoot);
			initRoot = nextRoot;
			count++;
		}

		return initRoot;
	}

	vector<complex<double>> FindComplexRoots()
	{
		vector<complex<double>> roots;
		vector<complex<double>> coefficients = this->coefficients;

		while (coefficients.size() > 1)
		{
			Polynomial<complex<double>> tempPoly(coefficients);
			complex<double> root = tempPoly.FindComplexRoot();
			roots.push_back(root);
			Polynomial<complex<double>> divResult = tempPoly.Divide(root);
			coefficients = divResult.Coefficients();
		}

		return roots;
	}

	/*
	* Возвращает все корни с их степенями
	*/
	vector<pair<int, complex<double>>> FindComplexRootsWithDegrees()
	{
		vector<pair<int, complex<double>>> roots;
		double epsilon = 1e-4;
		Polynomial<complex<double>> tempPoly(this->coefficients);

		while (tempPoly.Degree() >= 1)
		{
			int multipleDegree = 1;
			complex<double> root = tempPoly.FindComplexRoot();
			tempPoly = tempPoly.Divide(root);

			while (abs(tempPoly(root)) < epsilon && tempPoly.Degree() > 0)
			{
				multipleDegree++;
				tempPoly = tempPoly.Divide(root);
			}
			roots.push_back(make_pair(multipleDegree, root));
		}

		return roots;
	}

	/*
	* Возвращает случайное комплексное число
	* @return complex<double> - комплексное число
	*/
	complex<double> GetRandomComplexNumber()
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<> dis(-0.2, 0.2);
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

