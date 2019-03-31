#pragma once
#include <string>

using ull = unsigned long long;

// Класс длинных чисел
class LongPlus 
{
private:
	ull low, high;
public:
	LongPlus(ull n = 0);
	LongPlus(ull _low, ull _high);
	~LongPlus();
	LongPlus operator+(const LongPlus &_summand) const;
	std::string to_string();
	friend std::string to_string(const LongPlus &lpnum);
	static const ull e15 = 1E+15;
};

