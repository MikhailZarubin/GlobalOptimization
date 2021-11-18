#include "Function.h"

using std::string;


void Function::replaceExpression(const string& newExpression)
{
	functionExpression = newExpression;
}


long double Function::getValue(long double point) const
{
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

FunctionBorder::FunctionBorder(const string& expression, long double left, long double right) :Function(expression)
{
	if (right <= left)
		throw 0;

	leftBorder = left;
	rightBorder = right;
}

void FunctionBorder::replaceExpression(const std::string& newExpression, long double newLeft, long double newRight)
{
	if (newRight <= newLeft)
		throw 0;

	Function::replaceExpression(newExpression);
}

long double FunctionBorder::getValue(long double point) const
{
	if (point < leftBorder && point >= rightBorder)
		throw 0;

	return Function::getValue(point);
}

long double FunctionBorder::getLeftBorder() const
{
	return leftBorder;
}

long double FunctionBorder::getRightBorder() const
{
	return rightBorder;
}
