#pragma once

#include "../ComputingLibrary/Function.h"

#include <map>
#include <vector>
#include <set>

//task results at the point
using pointValues = std::pair<long double, std::vector<Function>::size_type>;

class IndexAlgorithm
{
	//original function
	FunctionBorder function;
	//constraint functions and coeff
	std::vector<std::pair<Function, long double>> constraintFunctions;
	//a lot of proven points (for the first step of the algorithm)
	std::set<std::pair<long double, pointValues>, bool(*) (const std::pair<long double, pointValues>&, const std::pair<long double, pointValues>&)> pointContainer;
	//the set of indices of points in which the condition was calculated under the corresponding number (Iv, for the second step of the algorithm)
	std::vector<std::set<long double>> indexContainer;
	//vector of the current lower bounds (for the third step of the algorithm)
	std::vector<long double> lowerBorder;
	//vector of the current Z* values (for the fourth step of the algorithm)
	std::vector<long double> zContainer;

	long double accuracy;
	long double coeff;

	//the first iteration with boundary points
	std::pair<std::pair<long double, pointValues>, std::pair<long double, pointValues>> startIteration();
	//get the result (Yash and M) at the point (for the first and second step of the algorithm)
	pointValues getTaskResult(long double point);
	//calculate the current lower bound (for the third step of the algorithm)
	void updateLowerBorder(std::vector<Function>::size_type index);
	//update the Z* values for all non-empty sets (for the fourth step of the algorithm)
	void updateValuesZ();
	//determine the interval for the next test (for the fifth and sixth steps of the algorithm)
	std::pair<std::pair<long double, pointValues>, std::pair<long double, pointValues>> calculateNewInterval();
	//calculate a new point based on the selected interval (for the seventh steps of the algorithm)
	long double calculateNewPoint(std::pair<std::pair<long double, pointValues>, std::pair<long double, pointValues>> newInterval);

	//cleaning from old values
	void clean();

public:
	IndexAlgorithm(const std::string& expression, const std::vector<std::pair<Function, long double>>& constraintsAndCoeff,
		long double leftBorder, long double rightBorder, long double accuracy_, long double coeff_ = 0.001);
	std::pair<std::pair<long double, long double>, int> startIndexAlgorithm();
};

