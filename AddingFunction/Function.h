#pragma once

#include "Calculator.h"

#include <set>

using std::set;
using std::pair;

using doublePair = pair<long double, long double>;

class Function
{
protected:
	long double leftBorder, rightBorder;
	string functionExpression;
	
public:
	Function() = delete;
	Function(const string& expression, long double left, long double right);
	void replaceExpression(const string& newExpression, long double newLeft, long double newRight);
	long double getValue(long double point) const;
};

class GlobalMinimumFunction: protected Function
{
	long double accuracy, rCoeff, maxDifference;
	doublePair globalMinimum;
	set<doublePair, bool(*) (const doublePair&, const doublePair&)> checkedCoordinates;

public:
	GlobalMinimumFunction() = delete;
	GlobalMinimumFunction(const string& expression, long double left, long double right, long double accur, long double rCoeff);
	void replaceAccuracy(long double newAccuracy);
	void replaceCoeff(long double newCoeff);
	doublePair searchGlobalMinimum();
};

