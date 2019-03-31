#include "LongPlusPlus.h"

LongPlusPlus LongPlusPlus::preMultCalc(const LongPlusPlus &_summand, const ull value, const int _pow = 0) const
{
	ull lowsum = 0;
	ull medsum = 0;
	ull highsum = 0;
	ull resultLow = 0;
	ull resultMedium = 0;
	ull resultHigh = 0;
	
	switch (_pow) 
	{
		case 0:
			lowsum = _summand.low * value;
			medsum = _summand.medium * value;
			highsum = _summand.high * value;
			break;
		case 1:
			medsum = _summand.low * value;
			highsum = _summand.high * value * this->e8 + _summand.medium * value;
			break;
		default:
			highsum = _summand.high * value * this->e8 * this->e8 + _summand.medium * value * this->e8 + _summand.low * value;
			break;
	}
					
	resultLow = lowsum % this->e8;
	resultMedium = (medsum + lowsum / this->e8) % this->e8;
	resultHigh = highsum + (medsum + lowsum / this->e8) / this->e8;

	return LongPlusPlus(resultLow, resultMedium, resultHigh);
}

LongPlusPlus::LongPlusPlus(ull n)
{
	this->low = n % this->e8;
	n = n / this->e8;
	this->medium = n % this->e8;
	n = n / this->e8;
	this->high = n;
}

LongPlusPlus::LongPlusPlus(ull _low, ull _medium, ull _high)
{
	this->low = _low;
	this->medium = _medium;
	this->high = _high;
}

std::string LongPlusPlus::addLeadingZeroes(std::string number)
{
	int constLength = std::to_string(LongPlusPlus::e8).length() - 2;
	while (number.length() <= constLength) {
		number = "0" + number;
	}
	return number;
}

LongPlusPlus LongPlusPlus::operator+(const LongPlusPlus & _summand) const
{
	ull lowSum = _summand.low + this->low;
	ull mediumSum = _summand.medium + this->medium;
	ull highSum = _summand.high + this->high;
	ull mediumProxy = mediumSum + lowSum / this->e8;
	
	ull resultLow = lowSum % this->e8;
	ull resultMedium = mediumProxy % this->e8;
	ull resultHigh = highSum + mediumProxy / this->e8;

	return LongPlusPlus(resultLow, resultMedium, resultHigh);
}

LongPlusPlus LongPlusPlus::operator*(const LongPlusPlus & _summand) const
{
	LongPlusPlus res1, res2, res3;
	res1 = preMultCalc(_summand, this->low, 0);
	res2 = preMultCalc(_summand, this->medium, 1);
	res3 = preMultCalc(_summand, this->high, 2);
	return res1 + res2 + res3;
}

std::string LongPlusPlus::to_string()
{
	std::string result = "";
	result += (this->high > 0) ? std::to_string(this->high) : "";
	result += (this->high > 0) ? this->addLeadingZeroes(std::to_string(this->medium)) : ((this->medium > 0) ? std::to_string(this->medium) : "");
	result += (this->medium > 0) ? this->addLeadingZeroes(std::to_string(this->low)) : ((this->low > 0) ? std::to_string(this->low) : "");
	return result;
}

LongPlusPlus::~LongPlusPlus()
{
}

std::string to_string(const LongPlusPlus & lpnum)
{
	std::string result = "";
	result += (lpnum.high > 0) ? std::to_string(lpnum.high) : "";
	result += (lpnum.high > 0) ? lpnum.addLeadingZeroes(std::to_string(lpnum.medium)) : ((lpnum.medium > 0) ? std::to_string(lpnum.medium) : "");
	result += (lpnum.medium > 0) ? lpnum.addLeadingZeroes(std::to_string(lpnum.low)) : ((lpnum.low > 0) ? std::to_string(lpnum.low) : "");
	return result;
}
