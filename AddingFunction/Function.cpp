#include "Function.h"

#include <algorithm>

namespace {
	bool comparisonPair(const doublePair& firstPair, const doublePair& secondPair)
	{
		return firstPair.second < secondPair.second;
	}
}


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


GlobalMinimumFunction::GlobalMinimumFunction(const string& expression, long double left, long double right, long double accur, long double coeff) : Function(expression, left, right), accuracy{ accur }, checkedCoordinates(::comparisonPair), maxDifference{ 0. }
{
	if (coeff <= 1.0)
		throw 0;

	rCoeff = coeff;

	long double valueFuncLeftBorder = getValue(leftBorder);
	long double valueFuncRightBorder = getValue(rightBorder);

	if (valueFuncLeftBorder <= valueFuncRightBorder)
		globalMinimum = { valueFuncLeftBorder, leftBorder };
	else
		globalMinimum = { valueFuncRightBorder, rightBorder };

	checkedCoordinates.insert({ valueFuncLeftBorder, leftBorder });
	checkedCoordinates.insert({ valueFuncRightBorder, rightBorder });
}


void GlobalMinimumFunction::replaceAccuracy(long double newAccuracy)
{
	accuracy = newAccuracy;
}


void GlobalMinimumFunction::replaceCoeff(long double newCoeff)
{
	if (newCoeff <= 1.0)
		throw 0;

	rCoeff = newCoeff;
}


doublePair GlobalMinimumFunction::searchGlobalMinimum()
{
	bool stopFlag = false;
	auto newElem = checkedCoordinates.end();

	while (!stopFlag)
	{
		for (auto iter = ++checkedCoordinates.begin(); iter != checkedCoordinates.end(); iter++)
		{
			doublePair currentElem = *(iter--);
			doublePair lastELem = *(iter++);

			long double difference = fabsl((currentElem.first - lastELem.first) / (currentElem.second - lastELem.second));
			maxDifference = std::max(maxDifference, difference);
		}

		long double m = rCoeff * maxDifference;

		if (m == 0)
			m = 1;

		long double maxValueInterval;
		auto desiredInterval = ++checkedCoordinates.begin();
		for (auto iter = ++checkedCoordinates.begin(); iter != checkedCoordinates.end(); iter++)
		{
			doublePair currentElem = *(iter--);
			doublePair lastELem = *(iter++);

			long double temporaryValue = m * (currentElem.second - lastELem.second);
			long double valueInterval = temporaryValue + (currentElem.first - lastELem.first) * (currentElem.first - lastELem.first) / temporaryValue - 2 * (currentElem.first + lastELem.first);

			if (iter == ++checkedCoordinates.begin() || valueInterval > maxValueInterval)
			{
				maxValueInterval = valueInterval;
				desiredInterval = iter;
			}
		}

		doublePair currentElemDesiredInterval = *(desiredInterval--);
		doublePair lastElemDesiredInterval = *(desiredInterval++);

		long double newCoordinate = (currentElemDesiredInterval.second + lastElemDesiredInterval.second) * 0.5 - (currentElemDesiredInterval.first - lastElemDesiredInterval.first) * 0.5 / m;
		long double newValue = getValue(newCoordinate);
		newElem = checkedCoordinates.insert({ newValue,newCoordinate }).first;

		if (newValue < globalMinimum.first)
			globalMinimum = { newValue, newCoordinate };

		stopFlag = currentElemDesiredInterval.second - lastElemDesiredInterval.second <= accuracy;
	}

	return globalMinimum;
}
