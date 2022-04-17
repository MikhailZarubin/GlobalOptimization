#pragma once

#include "Calculator.h"
#include <vector>


class Function
{
protected:
	std::string functionExpression;
	int dimension;
	std::string variableSet;
	
public:
	Function() = delete;
	Function(const std::string& expression, int dimension = 1, std::string variableSet = "");
	~Function() = default;
	void replaceExpression(const std::string& newExpression);
	long double getValue(long double point) const;
	long double getValue(std::vector<long double> point) const;
	std::string getExpression() const;
	int getDimension() const { return dimension; }
	std::string getVariableSet() const { return variableSet; };
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
	int getDimension() const = delete;
	std::string getVariableSet() const = delete;
};
