#include "Function.h"

#include <algorithm>

namespace {
	bool comparisonPair(const doublePair& firstPair, const doublePair& secondPair)
	{
		return firstPair.second < secondPair.second;
	}
}


Function::Function(const string& expression, double left, double right) : functionExpression{ expression }
{
	if (left >= right)
		throw 0;

	leftBorder = left;
	rightBorder = right;
}


void Function::replaceExpression(const string& newExpression, double newLeft, double newRight)
{
	if (newLeft >= newRight)
		throw 0;

	leftBorder = newLeft;
	rightBorder = newRight;

	functionExpression = newExpression;
}


double Function::getValue(double point) const
{
	if (point < leftBorder || point>rightBorder)
		throw 0;

	string expressionWithValue = functionExpression;
	string valuePoint = "(" + std::to_string(point) + ")";

	for (string::size_type index = 0; index < expressionWithValue.size(); index++)
	{
		if (expressionWithValue[index] == 't')
		{
			expressionWithValue.replace(index, 1, valuePoint);
			index += valuePoint.size() - 1;
		}
	}

	return Calculator(expressionWithValue).calculateResult();
}


GlobalMinimumFunction::GlobalMinimumFunction(const string& expression, double left, double right, double accur, double coeff) : Function(expression, left, right)
{
	if (coeff <= 1.0)
		throw 0;

	rCoeff = coeff;
	accuracy = accur;
}


void GlobalMinimumFunction::replaceAccuracy(double newAccuracy)
{
	accuracy = newAccuracy;
}


void GlobalMinimumFunction::replaceCoeff(double newCoeff)
{
	if (newCoeff <= 1.0)
		throw 0;

	rCoeff = newCoeff;
}
