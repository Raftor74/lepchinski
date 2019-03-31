#include "Longplus.h"

LongPlus::LongPlus(ull n)
{
	this->low = n % this->e15;
	this->high = n / this->e15;
}

LongPlus::LongPlus(ull _low, ull _high)
{
	this->low = _low;
	this->high = _high;
}


LongPlus LongPlus::operator+(const LongPlus &_summand) const
{
	ull lowsum = _summand.low + this->low;
	ull resultLow = (lowsum) % this->e15;
	ull resultHigh = _summand.high + this->high + lowsum / this->e15;
	return LongPlus(resultLow, resultHigh);
}

std::string LongPlus::to_string()
{
	return ((this->high > 0) ? std::to_string(this->high) : "") + std::to_string(this->low);
}

std::string to_string(const LongPlus &lpnum)
{
	return ((lpnum.high > 0) ? std::to_string(lpnum.high) : "") + std::to_string(lpnum.low);
}

LongPlus::~LongPlus()
{

}
