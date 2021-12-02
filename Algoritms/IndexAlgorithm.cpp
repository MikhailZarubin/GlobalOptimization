#include "IndexAlgorithm.h"

#include <fstream>

using std::string;
using std::pair;
using std::vector;
using std::set;

namespace {
	bool comparsion(const std::pair<long double, pointValues>& firstElem, const std::pair<long double, pointValues>& secondElem)
	{
		return firstElem.first < secondElem.first;
	}
}


IndexAlgorithm::IndexAlgorithm(const std::pair<std::string, long double>& expressionAndValue, const vector<std::pair<Function, long double>>& constraints,
	long double leftBorder, long double rightBorder, long double accuracy_, long double coeff_) :
	function(expressionAndValue.first, leftBorder, rightBorder),
	constraintFunctions(constraints),
	pointContainer(::comparsion),
	valueContainer(constraintFunctions.size() + 1, set<long double>()),
	indexContainer(constraintFunctions.size() + 1, set<long double>()),
	lowerBorder(constraintFunctions.size() + 1, 0),
	zContainer(constraintFunctions.size() + 1, 0),
	accuracy(accuracy_),
	coeff(coeff_)
{
	//for windows form
	std::ofstream functionsFile("inputFile.txt");
	functionsFile.clear();

	functionsFile << expressionAndValue.first << '\n' << leftBorder << '\n' << rightBorder << '\n' <<
		constraintFunctions.size() << '\n';

	for (const auto& constraint : constraintFunctions)
		functionsFile << constraint.first.getExpression() << '\n';

	functionsFile.close();
	//end windows form

	constraintFunctions.push_back({ expressionAndValue.first, expressionAndValue.second }); 
}

pair<pair<long double, pointValues>, pair<long double, pointValues>> IndexAlgorithm::startIteration()
{
	long double left = function.getLeftBorder();
	long double right = function.getRightBorder();

	auto taskResultLeftBorder = getTaskResult(left);
	auto taskResultRightBorder = getTaskResult(right);

	pointContainer.insert({ left, taskResultLeftBorder });
	pointContainer.insert({ right, taskResultRightBorder });

	updateValuesZ(std::max(taskResultRightBorder.second, taskResultLeftBorder.second));

	return calculateNewInterval();
}

pair<long double, vector<Function>::size_type> IndexAlgorithm::getTaskResult(long double point)
{
	for (vector<Function>::size_type i = 0; i < constraintFunctions.size() - 1; i++)
	{
		std::pair<std::set<long double>::iterator, bool> iterator = indexContainer[i].insert(point);
		valueContainer[i].insert(constraintFunctions[i].first.getValue(point));

		if (iterator.second)
			updateLowerBorder(i, iterator.first);

		if (auto value = constraintFunctions[i].first.getValue(point) > 0)
			return { value, i };
	}

	std::pair<std::set<long double>::iterator, bool> iterator = indexContainer[indexContainer.size() - 1].insert(point);
	valueContainer[indexContainer.size() - 1].insert(constraintFunctions[indexContainer.size() - 1].first.getValue(point));

	if (iterator.second)
		updateLowerBorder(indexContainer.size() - 1, iterator.first);

	return { function.getValue(point),  constraintFunctions.size() - 1 };
}

void IndexAlgorithm::updateLowerBorder(vector<Function>::size_type index, std::set<long double>::iterator iterNewElem)
{
	if (indexContainer[index].size() < 2)
	{
		lowerBorder[index] = 1;
		return;
	}

	long double newPoint = *iterNewElem;
	long double newValue = constraintFunctions[index].first.getValue(newPoint);

	for (auto iter = ++iterNewElem; iter != indexContainer[index].end(); ++iter)
	{
		long double point = *iter;
		long double value = constraintFunctions[index].first.getValue(point);

		lowerBorder[index] = std::max(lowerBorder[index], (value - newValue) / (point - newPoint));
	}

	for (auto iter = indexContainer[index].begin(); iter != iterNewElem; ++iter)
	{
		long double point = *iter;
		long double value = constraintFunctions[index].first.getValue(point);

		lowerBorder[index] = std::max(lowerBorder[index], (newValue - value) / (newPoint - point));
	}

	lowerBorder[index] = lowerBorder[index] == 0 ? 1 : lowerBorder[index];
}

void IndexAlgorithm::updateValuesZ(vector<Function>::size_type index)
{
	for (vector<Function>::size_type i = 0; i <= index; i++)
	{
		zContainer[i] = *valueContainer[i].begin();

		if (zContainer[i] <= 0)
			zContainer[i] = -lowerBorder[i] * coeff;
	}
}

pair<pair<long double, pointValues>, pair<long double, pointValues>> IndexAlgorithm::calculateNewInterval()
{
	auto lastIt = pointContainer.begin();
	pair<pair<long double, pointValues>, pair<long double, pointValues>> result;
	long double maxValue = 0;
	for (auto currentIt = ++pointContainer.begin(); currentIt != pointContainer.end(); currentIt++)
	{
		auto currentPoint = *currentIt;
		auto lastPoint = *lastIt;

		auto indexV = currentPoint.second.second;

		long double delta = currentPoint.first - lastPoint.first;
		long double currentValue;

		if (indexV == lastPoint.second.second)
		{
			currentValue = delta + (currentPoint.second.first -
				lastPoint.second.first) * (currentPoint.second.first - lastPoint.second.first) /
				(3 * 3 *
					lowerBorder[indexV] * lowerBorder[indexV] * delta) -
				2 * (currentPoint.second.first + lastPoint.second.first -
					2 * zContainer[indexV]) /
				(2 * lowerBorder[indexV]);
		}
		else if (indexV > lastPoint.second.second)
		{
			currentValue = 2 * delta - 4 * (currentPoint.second.first - zContainer[indexV]) /
				(2 * lowerBorder[indexV]);
		}
		else
		{
			indexV = lastPoint.second.second;
			currentValue = 2 * delta - 4 * (lastPoint.second.first - zContainer[indexV]) /
				(2 * lowerBorder[indexV]);
		}

		if (lastIt == pointContainer.begin() || maxValue < currentValue)
		{
			maxValue = currentValue;
			result = { lastPoint ,currentPoint };
		}

		lastIt = currentIt;
	}
	return result;
}

long double IndexAlgorithm::calculateNewPoint(pair<pair<long double, pointValues>, pair<long double, pointValues>> newInterval)
{
	if (newInterval.second.second.second != newInterval.first.second.second)
	{
		return (newInterval.first.first + newInterval.second.first) / 2;
	}
	else
	{
		auto indexV = newInterval.first.second.second;
		return (newInterval.first.first + newInterval.second.first) / 2 -
			(newInterval.second.second.first - newInterval.first.second.first) /
			(2 * 2 * lowerBorder[indexV]);
	}
}

pair<pair<long double, long double>, int> IndexAlgorithm::startIndexAlgorithm()
{
	std::ofstream pointFile("outputFile.txt");
	pointFile.clear();

	//cleaning from old values
	if (!pointContainer.empty())
		this->clean();

	pointFile << function.getLeftBorder();
	pointFile << function.getRightBorder();

	//calculation of a new interval by starting iteration
	auto newInterval = startIteration();
	//calculating a new point
	long double newPoint = calculateNewPoint(newInterval);

	int iterCount = 0;

	while (true)
	{
		//first and second and third steps
        auto taskResultNewPoint = getTaskResult(newPoint);
		pointContainer.insert({ newPoint, taskResultNewPoint });
		
		updateValuesZ(taskResultNewPoint.second);

		//fifth and sixth steps
		newInterval = calculateNewInterval();
		//seventh step
		newPoint = calculateNewPoint(newInterval);

		//for windows form
		pointFile << newPoint << '\n';

		//checking the stop condition
		if (newInterval.second.first - newInterval.first.first < accuracy)
			break;

		iterCount++;
	}
	pointFile.close();

	return { {function.getValue(newPoint), newPoint}, iterCount };
}

void IndexAlgorithm::clean()
{
	pointContainer.clear();
	indexContainer.clear();
	lowerBorder.clear();
	zContainer.clear();

	indexContainer = vector <set<long double>>(constraintFunctions.size(), set<long double>());
	lowerBorder = vector<long double>(constraintFunctions.size(), 0);
	zContainer = vector<long double>(constraintFunctions.size(), 0);
}
