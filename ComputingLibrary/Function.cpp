#include "Function.h"

using std::string;


Function::Function(const string& expression, long double left, long double right) : functionExpression{ expression }
{
	if (left >= right)
		throw 0;

	leftBorder = left;
	rightBorder = right;
}


void Function::replaceExpression(const string& newExpression, long double newLeft, long double newRight)
{
	if (newLeft >= newRight)
		throw 0;

	leftBorder = newLeft;
	rightBorder = newRight;

	functionExpression = newExpression;
}


long double Function::getValue(long double point) const
{
	if (point < leftBorder || point > rightBorder)
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

string Function::getExpression() const
{
	return functionExpression;
}
