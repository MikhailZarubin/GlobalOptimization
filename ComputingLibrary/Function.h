#pragma once

#include "Calculator.h"


class Function
{
protected:
	long double leftBorder, rightBorder;
	std::string functionExpression;
	
public:
	Function() = delete;
	Function(const std::string& expression, long double left, long double right);
	~Function() = default;
	void replaceExpression(const std::string& newExpression, long double newLeft, long double newRight);
	long double getValue(long double point) const;
	std::string getExpression() const;
};

