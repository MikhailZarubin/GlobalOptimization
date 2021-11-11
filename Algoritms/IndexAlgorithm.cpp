#include "IndexAlgorithm.h"

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


IndexAlgorithm::IndexAlgorithm(const string& expression, const vector<std::pair<Function, long double>>& constraints,
	long double leftBorder, long double rightBorder, long double accuracy_, long double coeff_) :
	function(expression, leftBorder, rightBorder),
	constraintFunctions(constraints),
	pointContainer(::comparsion),
	indexContainer(constraintFunctions.size() + 1, set<long double>()),
	lowerBorder(constraintFunctions.size() + 1, 0),
	zContainer(constraintFunctions.size() + 1, 0),
	accuracy(accuracy_),
	coeff(coeff_)
{
	constraintFunctions.push_back({ function, 2 }); //2 - random coeff
}

pair<pair<long double, pointValues>, pair<long double, pointValues>> IndexAlgorithm::startIteration()
{
	long double left = function.getLeftBorder();
	long double right = function.getRightBorder();

	auto taskResultLeftBorder = getTaskResult(left);
	auto taskResultRightBorder = getTaskResult(right);

	pointContainer.insert({ left, taskResultLeftBorder });
	pointContainer.insert({ right, taskResultRightBorder });

	updateLowerBorder(taskResultLeftBorder.second);
	updateLowerBorder(taskResultRightBorder.second);

	updateValuesZ();

	return calculateNewInterval();
}

pair<long double, vector<Function>::size_type> IndexAlgorithm::getTaskResult(long double point)
{
	for (vector<Function>::size_type i = 0; i < constraintFunctions.size() - 1; i++)
	{
		indexContainer[i].insert(point);
		if (auto value = constraintFunctions[i].first.getValue(point) > 0)
			return { value, i };
	}

	indexContainer[indexContainer.size() - 1].insert(point);
	return { function.getValue(point),  constraintFunctions.size() - 1 };
}

void IndexAlgorithm::updateLowerBorder(vector<Function>::size_type maxIndex)
{
	for (vector<Function>::size_type index = 0; index <= maxIndex; index++)
	{
		long double maxValue;

		if (indexContainer[index].size() < 2)
		{
			lowerBorder[index] = 0;
			break;
		}

		long double x1 = *(indexContainer[index].begin());
		long double x2 = *(++(indexContainer[index].begin()));

		maxValue = fabs(constraintFunctions[index].first.getValue(x2) - constraintFunctions[index].first.getValue(x1)) / (x2 - x1);

		for (auto it1 = ++(indexContainer[index].begin()); it1 != indexContainer[index].end(); it1++)
		{
			for (auto it2 = --it1; it2 != indexContainer[index].begin(); it2--)
			{
				maxValue = std::max(maxValue, fabs(constraintFunctions[index].first.getValue(*it1) - constraintFunctions[index].first.getValue(*it2)) / (*it1 - *it2));
			}
			++it1;
		}

		lowerBorder[index] = maxValue;
	}
}

void IndexAlgorithm::updateValuesZ()
{
	vector<long double>::size_type i = 0;
	for (const auto& pointSet : indexContainer)
	{
		if (pointSet.size() > 0)
		{
			zContainer[i] = constraintFunctions[i].first.getValue(*pointSet.begin());
			for (const auto& point : pointSet)
				zContainer[i] = std::min(zContainer[i], constraintFunctions[i].first.getValue(point));

			if (zContainer[i] <= 0)
			{
				updateLowerBorder(i);
				zContainer[i] = -(lowerBorder[i] * coeff);
			}
		}
		++i;
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
				(constraintFunctions[indexV].second * constraintFunctions[indexV].second *
					lowerBorder[indexV] * lowerBorder[indexV] * delta) -
				2 * (currentPoint.second.first + lastPoint.second.first -
					2 * zContainer[indexV]) /
				(constraintFunctions[indexV].second * lowerBorder[indexV]);
		}
		else if (indexV > lastPoint.second.second)
		{
			currentValue = 2 * delta - 4 * (currentPoint.second.first - zContainer[indexV]) /
				(constraintFunctions[indexV].second * lowerBorder[indexV]);
		}
		else
		{
			indexV = lastPoint.second.second;
			currentValue = 2 * delta - 4 * (lastPoint.second.first - zContainer[indexV]) /
				(constraintFunctions[indexV].second * lowerBorder[indexV]);
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
			(2 * constraintFunctions[indexV].second * lowerBorder[indexV]);
	}
}

pair<pair<long double, long double>, int> IndexAlgorithm::startIndexAlgorithm()
{
	//cleaning from old values
	if (!pointContainer.empty())
		this->clean();

	//calculation of a new interval by starting iteration
	auto newInterval = startIteration();
	//calculating a new point
	long double newPoint = calculateNewPoint(newInterval);

	int iterCount = 0;

	while (true)
	{
		//first and second steps
        auto taskResultNewPoint = getTaskResult(newPoint);
		pointContainer.insert({ newPoint, taskResultNewPoint });
		
		//third step
		updateLowerBorder(taskResultNewPoint.second);
		//fourth step
		updateValuesZ();

		//fifth and sixth steps
		newInterval = calculateNewInterval();
		//seventh step
		newPoint = calculateNewPoint(newInterval);

		//checking the stop condition
		if (newInterval.second.first - newInterval.first.first < accuracy)
			break;

		iterCount++;
	}
	return { {newPoint, function.getValue(newPoint)}, iterCount };
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
