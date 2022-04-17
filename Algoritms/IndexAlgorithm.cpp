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

	vector<long double> arrayToVector(long double* array, int arraySize) {
		vector<long double> result(arraySize, 0);
		for (vector<long double>::size_type i = 0; i < arraySize; i++) {
			result[i] = array[i];
		}
		return result;
	}

	vector<long double> linearTranformation(vector<long double> inputPoint, vector<long double> leftBorders, vector<long double> rightBorders) {
		vector<long double> newCoord(inputPoint.size(), 0);
		for (vector<long double>::size_type i = 0; i < newCoord.size(); i++) {
			newCoord[i] = leftBorders[i] + (inputPoint[i] + 0.5) * (rightBorders[i] - leftBorders[i]);
		}
		return newCoord;
	}

	long double sign(long double x) {
		return x > 0 ? 1 : (x < 0 ? -1 : 0);
	}
}


IndexAlgorithm::IndexAlgorithm(const string& expressionAndValue, const vector<Function>& constraints,
	long double leftBorder, long double rightBorder, long double _accuracy, long double coeff_) :
	function(expressionAndValue, leftBorder, rightBorder),
	constraintFunctions(constraints),
	pointContainer(::comparsion),
	valueContainer(constraintFunctions.size() + 1, set<long double>()),
	indexContainer(constraintFunctions.size() + 1, set<long double>()),
	lowerBorder(constraintFunctions.size() + 1, 0),
	zContainer(constraintFunctions.size() + 1, 0),
	accuracy(_accuracy),
	coeff(coeff_)
{
	constraintFunctions.push_back(expressionAndValue);
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
		if (auto value = constraintFunctions[i].getValue(point) > 0)
		{
			std::pair<std::set<long double>::iterator, bool> iterator = indexContainer[i].insert(point);
			valueContainer[i].insert(constraintFunctions[i].getValue(point));

			if (iterator.second)
				updateLowerBorder(i, iterator.first);

			return { value, i };
		}
	}

	std::pair<std::set<long double>::iterator, bool> iterator = indexContainer[indexContainer.size() - 1].insert(point);
	valueContainer[indexContainer.size() - 1].insert(constraintFunctions[indexContainer.size() - 1].getValue(point));

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
	long double newValue = constraintFunctions[index].getValue(newPoint);

	for (auto iter = ++iterNewElem; iter != indexContainer[index].end(); ++iter)
	{
		long double point = *iter;
		long double value = constraintFunctions[index].getValue(point);

		lowerBorder[index] = std::max(lowerBorder[index], fabs(value - newValue) / (point - newPoint));
	}

	for (auto iter = indexContainer[index].begin(); iter != iterNewElem; ++iter)
	{
		long double point = *iter;
		long double value = constraintFunctions[index].getValue(point);

		lowerBorder[index] = std::max(lowerBorder[index], fabs(newValue - value) / (newPoint - point));
	}

	lowerBorder[index] = fabs(lowerBorder[index]) < 0.001 ? 1 : lowerBorder[index];
}

void IndexAlgorithm::updateValuesZ(vector<Function>::size_type index)
{
	for (vector<Function>::size_type i = 0; i <= index; i++)
	{
		zContainer[i] = *valueContainer[i].begin();

		if (zContainer[i] <= 0)
			zContainer[i] = -lowerBorder[i] * 0.001;
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
				(coeff * coeff * lowerBorder[indexV] * lowerBorder[indexV] * delta) -
				2 * (currentPoint.second.first + lastPoint.second.first -
					2 * zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
		}
		else if (indexV > lastPoint.second.second)
		{
			currentValue = 2 * delta - 4 * (currentPoint.second.first - zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
		}
		else
		{
			indexV = lastPoint.second.second;
			currentValue = 2 * delta - 4 * (lastPoint.second.first - zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
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
			(2 * coeff * lowerBorder[indexV]);
	}
}

pair<pair<long double, long double>, int> IndexAlgorithm::startIndexAlgorithm()
{
	points.clear();

	//cleaning from old values
	if (!pointContainer.empty())
		this->clean();

	points.push_back(function.getLeftBorder());
	points.push_back(function.getRightBorder());

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
		points.push_back(newPoint);

		//checking the stop condition
		if (newInterval.second.first - newInterval.first.first < accuracy)
			break;

		iterCount++;
	}

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

std::vector<double> IndexAlgorithm::getPoints() const
{
	return points;
}

//IndexAlgorithmForMultidimensionalFunc

IndexAlgorithmForMultidimensionalFunc::IndexAlgorithmForMultidimensionalFunc(const string& expressionAndValue, const vector<Function>& constraints,
	vector<long double> _leftBorder, vector<long double> _rightBorder, int _dimension, const string& _variableSet, long double _accuracy, long double _coeff) :
	function(expressionAndValue, _dimension, _variableSet),
	leftBorder(_leftBorder),
	rightBorder(_rightBorder),
	constraintFunctions(constraints),
	pointContainer(::comparsion),
	valueContainer(constraintFunctions.size() + 1, set<long double>()),
	indexContainer(constraintFunctions.size() + 1, set<long double>()),
	lowerBorder(constraintFunctions.size() + 1, 0),
	zContainer(constraintFunctions.size() + 1, 0),
	accuracy(_accuracy),
	coeff(_coeff)
{
	constraintFunctions.push_back(Function(expressionAndValue, _dimension, _variableSet));
}

std::vector<long double> IndexAlgorithmForMultidimensionalFunc::parseArguments(long double basicPoint) {
	long double* argumentForPoint = new long double[function.getDimension()];
	mapd(basicPoint, 10, argumentForPoint, function.getDimension(), 1);

	vector<long double> vectorArgumentForPointBasic = ::arrayToVector(argumentForPoint, function.getDimension());
    vector<long double> vectorArgumentForPointLinear = ::linearTranformation(vectorArgumentForPointBasic, leftBorder, rightBorder);

	delete[] argumentForPoint;

	return vectorArgumentForPointLinear;
}

pair<pair<long double, pointValues>, pair<long double, pointValues>> IndexAlgorithmForMultidimensionalFunc::startIteration()
{
	auto taskResultLeftBorder = getTaskResult(0);
	auto taskResultRightBorder = getTaskResult(1);

	pointContainer.insert({ 0, taskResultLeftBorder });
	pointContainer.insert({ 1, taskResultRightBorder });

	updateValuesZ(std::max(taskResultRightBorder.second, taskResultLeftBorder.second));

	return calculateNewInterval();
}

pair<long double, vector<Function>::size_type> IndexAlgorithmForMultidimensionalFunc::getTaskResult(long double point)
{
	std::vector<long double> arguments = parseArguments(point);

	for (vector<Function>::size_type i = 0; i < constraintFunctions.size() - 1; i++)
	{
		if (auto value = constraintFunctions[i].getValue(arguments) > 0)
		{
			pair<set<long double>::iterator, bool> iterator = indexContainer[i].insert(point);
			valueContainer[i].insert(constraintFunctions[i].getValue(arguments));

			if (iterator.second)
				updateLowerBorder(i, iterator.first);

			return { value, i };
		}
	}

	pair<set<long double>::iterator, bool> iterator = indexContainer[indexContainer.size() - 1].insert(point);
	valueContainer[indexContainer.size() - 1].insert(constraintFunctions[indexContainer.size() - 1].getValue(arguments));

	if (iterator.second)
		updateLowerBorder(indexContainer.size() - 1, iterator.first);

	return { function.getValue(arguments),  constraintFunctions.size() - 1 };
}

void IndexAlgorithmForMultidimensionalFunc::updateLowerBorder(vector<Function>::size_type index, std::set<long double>::iterator iterNewElem)
{
	if (indexContainer[index].size() < 2)
	{
		lowerBorder[index] = 1;
		return;
	}

	long double newPoint = *iterNewElem;
	std::vector<long double> arguments = parseArguments(newPoint);
	long double newValue = constraintFunctions[index].getValue(arguments);

	for (auto iter = ++iterNewElem; iter != indexContainer[index].end(); ++iter)
	{
		long double point = *iter;
		arguments = parseArguments(newPoint);
		long double value = constraintFunctions[index].getValue(arguments);

		lowerBorder[index] = std::max(lowerBorder[index], fabs(value - newValue) / pow((point - newPoint), 1 / function.getDimension()));
	}

	for (auto iter = indexContainer[index].begin(); iter != iterNewElem; ++iter)
	{
		long double point = *iter;
		arguments = parseArguments(newPoint);
		long double value = constraintFunctions[index].getValue(arguments);

		lowerBorder[index] = std::max(lowerBorder[index], fabs(newValue - value) / pow((point - newPoint), 1 / function.getDimension()));
	}

	lowerBorder[index] = fabs(lowerBorder[index]) < 0.001 ? 1 : lowerBorder[index];
}

void IndexAlgorithmForMultidimensionalFunc::updateValuesZ(vector<Function>::size_type index)
{
	for (vector<Function>::size_type i = 0; i <= index; i++)
	{
		zContainer[i] = *valueContainer[i].begin();

		if (zContainer[i] <= 0)
			zContainer[i] = -lowerBorder[i] * 0.001;
	}
}

pair<pair<long double, pointValues>, pair<long double, pointValues>> IndexAlgorithmForMultidimensionalFunc::calculateNewInterval()
{
	auto lastIt = pointContainer.begin();
	pair<pair<long double, pointValues>, pair<long double, pointValues>> result;
	long double maxValue = 0;
	for (auto currentIt = ++pointContainer.begin(); currentIt != pointContainer.end(); currentIt++)
	{
		auto currentPoint = *currentIt;
		auto lastPoint = *lastIt;

		auto indexV = currentPoint.second.second;

		long double delta = pow(currentPoint.first - lastPoint.first, 1 / function.getDimension());
		long double currentValue;

		if (indexV == lastPoint.second.second)
		{
			currentValue = delta + (currentPoint.second.first -
				lastPoint.second.first) * (currentPoint.second.first - lastPoint.second.first) /
				(coeff * coeff * lowerBorder[indexV] * lowerBorder[indexV] * delta) -
				2 * (currentPoint.second.first + lastPoint.second.first -
					2 * zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
		}
		else if (indexV > lastPoint.second.second)
		{
			currentValue = 2 * delta - 4 * (currentPoint.second.first - zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
		}
		else
		{
			indexV = lastPoint.second.second;
			currentValue = 2 * delta - 4 * (lastPoint.second.first - zContainer[indexV]) /
				(coeff * lowerBorder[indexV]);
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

long double IndexAlgorithmForMultidimensionalFunc::calculateNewPoint(pair<pair<long double, pointValues>, pair<long double, pointValues>> newInterval)
{
	if (newInterval.second.second.second != newInterval.first.second.second)
	{
		return (newInterval.first.first + newInterval.second.first) / 2;
	}
	else
	{
		auto indexV = newInterval.first.second.second;
		return (newInterval.first.first + newInterval.second.first) / 2 -
			sign(newInterval.second.second.first - newInterval.first.second.first) * 
			(1 / (2 * coeff)) * pow(fabs(newInterval.second.second.first - newInterval.first.second.first) / lowerBorder[indexV], function.getDimension());
	}
}

pair<pair<long double, long double>, int> IndexAlgorithmForMultidimensionalFunc::startIndexAlgorithm()
{
	points.clear();

	//cleaning from old values
	if (!pointContainer.empty())
		this->clean();

	points.push_back(0);
	points.push_back(1);

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
		points.push_back(newPoint);

		//checking the stop condition
		long double delta = pow(newInterval.second.first - newInterval.first.first, function.getDimension());
		if (delta < accuracy)
			break;

		iterCount++;
	}

	std::vector<long double> arguments = parseArguments(newPoint);

	return { {function.getValue(arguments), newPoint}, iterCount };
}

void IndexAlgorithmForMultidimensionalFunc::clean()
{
	pointContainer.clear();
	indexContainer.clear();
	lowerBorder.clear();
	zContainer.clear();

	indexContainer = vector <set<long double>>(constraintFunctions.size(), set<long double>());
	lowerBorder = vector<long double>(constraintFunctions.size(), 0);
	zContainer = vector<long double>(constraintFunctions.size(), 0);
}

std::vector<double> IndexAlgorithmForMultidimensionalFunc::getPoints() const
{
	return points;
}
