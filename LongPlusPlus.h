#pragma once
#include <string>
using ull = unsigned long long;

class LongPlusPlus
{
private:
	ull low, medium, high;
	LongPlusPlus preMultCalc(const LongPlusPlus &_summand, const ull value, const int _pow) const;
public:
	LongPlusPlus(ull n = 0);
	LongPlusPlus(ull _low, ull _medium, ull _high);

	LongPlusPlus operator+(const LongPlusPlus &_summand) const;
	LongPlusPlus operator*(const LongPlusPlus &_summand) const;
	std::string to_string();
	friend std::string to_string(const LongPlusPlus &lpnum);

	~LongPlusPlus();
	static const ull e8 = 1E+8;
	static std::string addLeadingZeroes(std::string number);
};

