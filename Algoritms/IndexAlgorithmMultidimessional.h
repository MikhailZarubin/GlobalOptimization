#pragma once

#include "../ComputingLibrary/Function.h"

#include <map>
#include <vector>
#include <set>

struct optimalPoint {
	long double value;
	std::vector<long double> arguments;

	optimalPoint() : value(0), arguments(0) {};
	optimalPoint(long double _value, const std::vector<long double> _arguments) : value(_value), arguments(_arguments) {};
	optimalPoint& operator = (const optimalPoint obj) { value = obj.value; arguments = obj.arguments; return *this; }
};

struct taskResultStruct {
	std::vector<Function>::size_type v;
	long double z;

	taskResultStruct() : v(0), z(0) {};
	taskResultStruct(std::vector<Function>::size_type _v, long double _z) : v(_v), z(_z) {};
	taskResultStruct& operator = (const taskResultStruct& obj) { v = obj.v; z = obj.z; return *this; }
};

struct domain {
	long double leftBorder;
	long double rightBorder;

	domain() : leftBorder(0), rightBorder(0) {};
	domain(long double left, long double right) : leftBorder(left), rightBorder(right) {};
	domain& operator = (const domain& obj) { leftBorder = obj.leftBorder; rightBorder = obj.rightBorder; return *this; };
};

struct updatedV {
	std::vector<Function>::size_type v;
	std::set<long double>::iterator iterator;

	updatedV(std::vector<Function>::size_type _v, std::set<long double>::iterator _iterator) :
		v(_v),
		iterator(_iterator)
	{};
};

class IndexAlgorithmMultidimessional {
	Function function;
	std::vector<domain> borders;
	std::vector<Function> constraintFunctions;
	long double accuracy;
	long double rCoeff;

	std::set<long double> peanoPoints;
	std::map<long double, taskResultStruct> taskResult;
	std::map<std::vector<Function>::size_type, std::set<long double>> pointClassification;
	std::vector<long double> lowerBounds;
	std::vector<Function>::size_type M;

	bool stopCheck;

	int iterCount;
	std::vector<std::vector<long double>> points;

	long double startIteration();
	std::vector<long double> parseArg(long double peanoX);
	updatedV calculateTaskResult(long double peanoX);
	void calculateLowerBounds(updatedV newV);
	std::vector<long double> calculateMarks();
	long double calculateNewPoint(const std::vector<long double>& marks);

public:
	IndexAlgorithmMultidimessional(const Function& func, const std::vector<Function>& constraintFunc, 
		const std::vector<domain>& borders, long double accur = 0.001, long double rCoeff = 2);
	optimalPoint run();
	
	std::vector<std::vector<long double>> getPoints() const { return points; };
	int getIterCount() const { return iterCount; }
};

