#pragma once
#include <cassert>

template<typename T>
class Matrix
{
	size_t _n = 0;
	size_t _m = 0;
	T* _data = nullptr;
public:
	Matrix(size_t N, size_t M) {
		assert((N > 0) && (N > 0));
		_data = new T[M * N];
	};

	Matrix(Matrix& const m) {
		_n = m._n;
		_m = m._m;
		_data = new T[_m * _n];
		std::memcpy(_data, m._data, _m * _n * sizeof(T));
	};

	Matrix& operator =(Matrix& const m) {
		if (_data == m._data)
			return *this;

		if (_data != nullptr)
			delete[] _data;

		this(m);
		return *this;
	}

	~Matrix() {
		if (_data != nullptr) {
			delete[] _data;
		}
	}

	const auto& operator()(size_t i, size_t j) const {
		assert(i >= 0 && i < _n);
		assert(j >= 0 && j < _m);
		return _data[i*_n + _m];
	};

	auto& operator()(size_t i, size_t j) {
		assert(i >= 0 && i < _n);
		assert(j >= 0 && j < _m);
		return _data[i*_n + j*_m];
	};

	Matrix<T> operator+(const Matrix<T>& rhs) {
		assert(this->_n == rhs->_n && this->_m == rhs->_m);
		Matrix result(this->_n, this->_m);
		for (size_t i = 0; i < this->_n; i++) {
			for (size_t j = 0; j < this->_m; j++) {
				result(i, j) = this->_data[i + j] + rhs(i, j);
			}
		}
		return result;
	}
};


