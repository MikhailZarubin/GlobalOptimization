#pragma once

#include "Calculator.h"

#include <set>

using std::set;
using std::pair;

using doublePair = pair<double, double>;

class Function
{
protected:
	double leftBorder, rightBorder;
	string functionExpression;
	
public:
	Function() = delete;
	Function(const string& expression, double left, double right);
	void replaceExpression(const string& newExpression, double newLeft, double newRight);
	double getValue(double point) const;
};

class GlobalMinimumFunction: public Function
{
	double accuracy, rCoeff;
	set<doublePair, bool(*) (const doublePair&, const doublePair&)> checkedCoordinates;

public:
	GlobalMinimumFunction() = delete;
	GlobalMinimumFunction(const string& expression, double left, double right, double accur, double rCoeff);
	void replaceAccuracy(double newAccuracy);
	void replaceCoeff(double newCoeff);
	double searchGlobalMinimum();
};

