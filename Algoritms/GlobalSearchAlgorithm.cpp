#include "GlobalSearchAlgorithm.h"

using std::string;

namespace {
	bool comparisonPair(const doublePair& firstPair, const doublePair& secondPair)
	{
		return firstPair.second < secondPair.second;
	}
}


GlobalSearch::GlobalSearch(const string& expression, long double left, long double right, long double accur, long double coeff) : FunctionBorder(expression, left, right), accuracy{ accur }, checkedCoordinates(::comparisonPair), maxDifference{ 0. }
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


void GlobalSearch::replaceAccuracy(long double newAccuracy)
{
	accuracy = newAccuracy;
}


void GlobalSearch::replaceCoeff(long double newCoeff)
{
	if (newCoeff <= 1.0)
		throw 0;

	rCoeff = newCoeff;
}


std::pair<doublePair,int> GlobalSearch::searchGlobalMinimum()
{
	bool stopFlag = false;

	while (!stopFlag)
	{
		for (auto iter = ++checkedCoordinates.begin(); iter != checkedCoordinates.end(); iter++)
		{
			doublePair currentElem = *(iter--);
			doublePair lastELem = *(iter++);

			long double difference = abs((currentElem.first - lastELem.first) / (currentElem.second - lastELem.second));
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
		checkedCoordinates.insert({ newValue,newCoordinate });

		if (newValue < globalMinimum.first)
			globalMinimum = { newValue, newCoordinate };

		stopFlag = currentElemDesiredInterval.second - lastElemDesiredInterval.second <= accuracy;
	}

	return { globalMinimum, checkedCoordinates.size() };
}
