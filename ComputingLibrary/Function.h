#pragma once

#include "Calculator.h"


class Function
{
protected:
	std::string functionExpression;
	
public:
	Function() = delete;
	Function(const std::string& expression) :functionExpression{ expression } {};
	~Function() = default;
	void replaceExpression(const std::string& newExpression);
	long double getValue(long double point) const;
	std::string getExpression() const;
};

class FunctionBorder : public Function
{
protected:
	long double leftBorder, rightBorder;

public:
	FunctionBorder(const std::string& expression, long double left, long double right);
	void replaceExpression(const std::string& newExpression, long double newLeft, long double newRight);
	long double getValue(long double point) const;
	long double getLeftBorder() const;
	long double getRightBorder() const;
};
