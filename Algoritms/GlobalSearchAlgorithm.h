#pragma once

#include "../ComputingLibrary/Function.h"
#include <set>

using doublePair = std::pair<long double, long double>;


class GlobalSearch: protected Function
{
	long double accuracy, rCoeff, maxDifference;
	doublePair globalMinimum;
	std::set<doublePair, bool(*) (const doublePair&, const doublePair&)> checkedCoordinates;

public:
	GlobalSearch() = delete;
	GlobalSearch(const std::string& expression, long double left, long double right, long double accur, long double rCoeff);
	void replaceAccuracy(long double newAccuracy);
	void replaceCoeff(long double newCoeff);
	std::pair<std::pair<long double, long double>, int> searchGlobalMinimum();
};

