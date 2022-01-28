#pragma once

#include "../ComputingLibrary/Function.h"
#include <vector>
#include <set>

using doublePair = std::pair<long double, long double>;


class GlobalSearch
{
	FunctionBorder function;
	long double accuracy, rCoeff, maxDifference;
	doublePair globalMinimum;
	std::set<doublePair, bool(*) (const doublePair&, const doublePair&)> checkedCoordinates;
	std::vector<double> points;

	void startIteration();

public:
	GlobalSearch() = delete;
	GlobalSearch(const std::string& expression, long double left, long double right, long double accur, long double rCoeff);
	void replaceAccuracy(long double newAccuracy);
	void replaceCoeff(long double newCoeff);
	std::pair<std::pair<long double, long double>, int> searchGlobalMinimum();
	std::vector<double> getPoints() const;
};

