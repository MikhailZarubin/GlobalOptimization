#include "IndexAlgorithmMultidimessional.h"
#include "../ComputingLibrary/Legacy_x_to_y.h"


namespace {
	std::vector<long double> arrayToVector(long double* array,  int size) {
		std::vector<long double> vector(size);
		for (std::vector<long double>::size_type i = 0; i < size; i++) {
			vector[i] = array[i];
		}
		return vector;
	}

	std::vector<long double> linearTransform(const std::vector<long double>& inputArg, const std::vector<domain>& borders) {
		std::vector<long double> outputArg(inputArg.size());
		for (std::vector<long double>::size_type i = 0; i < outputArg.size(); i++) {
			outputArg[i] = borders[i].leftBorder + (inputArg[i] + 0.5) * (borders[i].rightBorder - borders[i].leftBorder);
		}
		return outputArg;
	}

	long double sign(long double x) {
		return x < 0.0 ? -1.0 : (x > 0.0 ? 1.0 : 0.0);
	}
}


IndexAlgorithmMultidimessional::IndexAlgorithmMultidimessional(const Function& func, const std::vector<Function>& constraintFunc,
	const std::vector<domain>& _borders, long double _accuracy, long double _rCoeff) :
	function(func),
	constraintFunctions(constraintFunc),
	lowerBounds(constraintFunc.size() + 1, -1),
	borders(_borders),
	accuracy(_accuracy),
    rCoeff (_rCoeff),
	stopCheck(false),
    M(0),
	iterCount(0) {
	constraintFunctions.push_back(function);
}

std::vector<long double> IndexAlgorithmMultidimessional::parseArg(long double peanoX) {
	if (peanoX < 0 || peanoX > 1)
		throw peanoX;

	long double* y = new long double[function.getDimension()];
	mapd(peanoX, 10, y, function.getDimension(), 1);

	auto arg = ::arrayToVector(y, function.getDimension());
	auto linearArg = ::linearTransform(arg, borders);

	delete[] y;

	return linearArg;
}

long double IndexAlgorithmMultidimessional::startIteration() {
	long double firstStartPoint = 0.4;
	long double secondStartPoint = 0.6;

	peanoPoints.insert(firstStartPoint);
	peanoPoints.insert(secondStartPoint);

	auto updatedV0 = calculateTaskResult(firstStartPoint);
	auto updatedV1 = calculateTaskResult(secondStartPoint);

	calculateLowerBounds(updatedV0);
	calculateLowerBounds(updatedV1);
	auto marks = calculateMarks();
	
	return calculateNewPoint(marks);
}

updatedV IndexAlgorithmMultidimessional::calculateTaskResult(long double peanoX) {
	std::vector<long double> arg = parseArg(peanoX);

	for (std::vector<Function>::size_type v = 0; v < constraintFunctions.size(); v++) {
		auto z = constraintFunctions[v].getValue(arg);
		if (z > 0 || v == constraintFunctions.size() - 1) {
			taskResult[peanoX] = taskResultStruct(v, z);
			auto updatedIterator = pointClassification[v].insert(peanoX);
			M = std::max(M, v);
			return updatedV(v, updatedIterator.first);
		}
	}
}

void IndexAlgorithmMultidimessional::calculateLowerBounds(updatedV newV) {
	auto v = newV.v;
	auto iterator = newV.iterator;

	for (auto i = pointClassification[v].begin(); i != iterator; i++) {
		long double xI = *i;
		long double xV = *iterator;

		long double zI = taskResult[xI].z;
		long double zV = taskResult[xV].z;

		lowerBounds[v] = std::max(lowerBounds[v], fabs(zV - zI) / powl(xV - xI, 1.0 / function.getDimension()));
	}

	for (auto i = ++iterator; i != pointClassification[v].end(); i++) {
		long double xI = *i;
		long double xV = *iterator;

		long double zI = taskResult[xI].z;
		long double zV = taskResult[xV].z;

		lowerBounds[v] = std::max(lowerBounds[v], fabs(zI - zV) / powl(xI - xV, 1.0 / function.getDimension()));
	}

	if (lowerBounds[v] <= 0) lowerBounds[v] = 1;
}

std::vector<long double> IndexAlgorithmMultidimessional::calculateMarks() {
	std::vector<long double> marks(constraintFunctions.size(), -1);

	for (std::vector<Function>::size_type v = 0; v <= M; v++) {
		if (v == M) {
			long double minValue;
			for (auto iter = pointClassification[v].begin(); iter != pointClassification[v].end(); iter++) {
				long double value = taskResult[*iter].z;

				if (iter == pointClassification[v].begin()) {
					minValue = value;
				}
				else {
					minValue = std::min(value, minValue);
				}
			}
			marks[v] = minValue;
		}

		else
			marks[v] = -lowerBounds[v] * 0.001;
	}

	return marks;
}

long double IndexAlgorithmMultidimessional::calculateNewPoint(const std::vector<long double>& marks) {
	std::pair<long double, long double> newInterval;
	long double maxR;

	auto previousPointIter = peanoPoints.begin();
	for (auto currentPointIter = ++peanoPoints.begin(); currentPointIter != peanoPoints.end(); currentPointIter++) {
		long double previousPoint = *previousPointIter;
		long double currentPoint = *currentPointIter;

		long double delta = powl(currentPoint - previousPoint, 1.0 / function.getDimension());
		long double R;

		auto previousPointZ = taskResult[previousPoint].z;
		auto previousPointV = taskResult[previousPoint].v;
		
		auto currentPointZ = taskResult[currentPoint].z;
		auto currentPointV = taskResult[currentPoint].v;

		if (previousPointV == currentPointV) {
			R = delta + pow((currentPointZ - previousPointZ), 2) / (pow(rCoeff, 2) * pow(lowerBounds[currentPointV], 2) * delta) -
				2 * (currentPointZ + previousPointZ - 2 * marks[currentPointV]) / (lowerBounds[currentPointV] * rCoeff);
		}
		else if (currentPointV > previousPointV) {
			R = 2 * delta - 4 * (currentPointZ - marks[currentPointV]) / (lowerBounds[currentPointV] * rCoeff);
		}
		else {
			R = 2 * delta - 4 * (previousPointZ - marks[previousPointV]) / (lowerBounds[previousPointV] * rCoeff);
		}

		if (previousPointIter == peanoPoints.begin()) {
			maxR = R;
			newInterval = { previousPoint, currentPoint };
		}
		else if (R > maxR) {
			maxR = R;
			newInterval = { previousPoint, currentPoint };
		}

		previousPointIter = currentPointIter;
	}

	stopCheck = pow(newInterval.second - newInterval.first, 1.0 / function.getDimension()) <= accuracy;

	if (taskResult[newInterval.first].v != taskResult[newInterval.second].v) {
		return (newInterval.first + newInterval.second) / 2;
	}
	else {
		long double lastPointZ = taskResult[newInterval.first].z;
		long double currentPointZ = taskResult[newInterval.second].z;

		long double v = taskResult[newInterval.second].v;

		auto test = (newInterval.first + newInterval.second) / 2;

		return (newInterval.first + newInterval.second) / 2 - sign(currentPointZ - lastPointZ) *
			(1 / (2 * rCoeff)) * pow( fabsl(currentPointZ - lastPointZ) / lowerBounds[v], function.getDimension());
	}
}

optimalPoint IndexAlgorithmMultidimessional::run() {
	iterCount = 2;
	auto newPoint = startIteration();

	while (!stopCheck) {
		peanoPoints.insert(newPoint);
		auto updatedV = calculateTaskResult(newPoint);
		calculateLowerBounds(updatedV);
		auto marks = calculateMarks();
		
		newPoint = calculateNewPoint(marks);
		iterCount++;
	}

	auto arguments = parseArg(newPoint);
	auto value = function.getValue(arguments);

	return optimalPoint(value, arguments);
}
