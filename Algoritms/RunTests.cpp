#include "GlobalSearchAlgorithm.h"
#include "IndexAlgorithm.h"

#include <iostream>
#include <fstream>

using std::string;
using std::vector;
using std::pair;

const int bufferSize = 250;
char buffer[bufferSize];

void initTestFuncGSA(const char fileName[], double accuracy, double rCoeff)
{
	std::ifstream file(fileName);

	string expression;
	vector<pair<Function, long double>> conditionsContainer;

	double leftBorder, rightBorder, expectedResult, expectedCoordinate;

	if (file)
	{
		while (!file.eof())
		{
			file.getline(buffer, bufferSize, '\n');
			expression = buffer;

			file.getline(buffer, bufferSize, '\n');
			leftBorder = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			rightBorder = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			expectedResult = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			expectedCoordinate = atof(buffer);

			GlobalSearch testFunc(expression, leftBorder, rightBorder, accuracy, rCoeff);
			std::pair<doublePair, int> res = testFunc.searchGlobalMinimum();

			std::cout << "\nFUNC: " << expression << "\nLEFT_BORDER: " << leftBorder << "\nRIGHT_BORDER: " << rightBorder << "\nACCURACY: " << accuracy << "\nR_COEFF: " << rCoeff
				<< "\nMIN VALUE: " << res.first.first << "\nCOORDINATE: " << res.first.second << "\nITER_COUNT: " << res.second
				<< "\nEXPECTED_RESULT: " << expectedResult << "\nEXPECTED_COORDINATE: " << expectedCoordinate << "\nERROR: " << fabsl(res.first.first - expectedResult) << '\n';
		}
		file.close();
	}
	else
		throw false;
}

void initTestFuncIA(const char fileName[], double accuracy)
{
	std::ifstream file(fileName);

	string expression;
	int countConditions;
	vector<pair<Function, long double>> conditionsContainer;
	long double coeffForFunctions, leftBorder, rightBorder, expectedResult, expectedCoordinate;

	if (file)
	{
		while (!file.eof())
		{
			file.getline(buffer, bufferSize, '\n');
			expression = buffer;

			file.getline(buffer, bufferSize, '\n');
			coeffForFunctions = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			leftBorder = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			rightBorder = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			countConditions = atoi(buffer);

			for (int i = 0; i < countConditions; i++)
			{
				file.getline(buffer, bufferSize, '\n');
				string conditionsNumberI = buffer;

				file.getline(buffer, bufferSize, '\n');
				long double coeffNumberI = atof(buffer);

				conditionsContainer.push_back({ Function(conditionsNumberI), coeffNumberI });
			}

			file.getline(buffer, bufferSize, '\n');
			expectedResult = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			expectedCoordinate = atof(buffer);

			IndexAlgorithm testFunc({ expression, coeffForFunctions }, conditionsContainer, leftBorder, rightBorder, accuracy);
			auto res = testFunc.startIndexAlgorithm();

			std::cout << "\nFUNC: " << expression << "\nLEFT_BORDER: " << leftBorder << "\nRIGHT_BORDER: " << rightBorder << "\nACCURACY: " << accuracy
				<< "\nMIN VALUE: " << res.first.first << "\nCOORDINATE: " << res.first.second << "\nITER_COUNT: " << res.second
				<< "\nEXPECTED_RESULT: " << expectedResult << "\nEXPECTED_COORDINATE: " << expectedCoordinate << "\nERROR: " << fabsl(res.first.first - expectedResult) << '\n';

			conditionsContainer.clear();
		}
		file.close();
	}
	else
		throw false;
}


int main()
{
	int testStrategy{ 0 }, algorithmNumber{ 0 };

	double leftBorder, rightBorder, accuracy, r;
	string expression;

	while (algorithmNumber < 3)
	{
		std::cout << "\n1. Global Search Algorithm\n2. Index Algorithm\n3. Exit\n";
		std::cin >> algorithmNumber;
		testStrategy = 0;

		if (algorithmNumber == 1)
		{
			std::cout << "\n1. Manual input\n2. Testing Functions\n";
			std::cin >> testStrategy;

			if (testStrategy == 1)
			{
				std::cout << "FUNC(t): ";
				std::cin >> expression;

				std::cout << "LEFT_BORDER: ";
				std::cin >> leftBorder;
				std::cout << "RIGHT_BORDER: ";
				std::cin >> rightBorder;

				std::cout << "ACCURACY: ";
				std::cin >> accuracy;
				std::cout << "R_COEFF: ";
				std::cin >> r;

				GlobalSearch testFunc(expression, leftBorder, rightBorder, accuracy, r);
				std::pair<doublePair, int> res = testFunc.searchGlobalMinimum();

				std::cout << "MIN VALUE: " << res.first.first << '\n' << "COORDINATE: " << res.first.second << '\n' << "ITER_COUNT: " << res.second << '\n';
			}

			else if (testStrategy == 2)
			{
				try
				{
					std::cout << "ACCURACY: ";
					std::cin >> accuracy;

					std::cout << "R_COEFF: ";
					std::cin >> r;

					initTestFuncGSA("../TestingFunctions/globalSearchAlgorithm.txt", accuracy, r);
				}
				catch (bool)
				{
					std::cout << "FILE_ERROR";
				}
			}

			else
				break;
		}

		else if (algorithmNumber == 2)
		{
			std::cout << "\n1. Manual input\n2. Testing Functions\n";
			std::cin >> testStrategy;

			if (testStrategy == 1)
			{
				long double coeffForFunctions;

				std::cout << "FUNC(t): ";
				std::cin >> expression;

				std::cout << "FUNC COEFF: ";
				std::cin >> coeffForFunctions;

				std::cout << "LEFT_BORDER: ";
				std::cin >> leftBorder;
				std::cout << "RIGHT_BORDER: ";
				std::cin >> rightBorder;

				vector<FunctionBorder>::size_type countConditions;
				std::cout << "COUNT CONDITIONS:";
				std::cin >> countConditions;

				vector<pair<Function, long double>> conditionsContainer;
				for (vector<FunctionBorder>::size_type i = 0; i < countConditions; i++)
				{
					string conditionsNumberI;
					long double coeffNumberI;

					std::cout << "CONDITION " << i << ":";
					std::cin >> conditionsNumberI;

					std::cout << "COEFF " << i << ":";
					std::cin >> coeffNumberI;

					conditionsContainer.push_back({ Function(conditionsNumberI), coeffNumberI });
				}

				std::cout << "ACCURACY: ";
				std::cin >> accuracy;

				IndexAlgorithm testFunc({ expression, coeffForFunctions }, conditionsContainer, leftBorder, rightBorder, accuracy);
				auto res = testFunc.startIndexAlgorithm();

				std::cout << "MIN VALUE: " << res.first.first << '\n' << "COORDINATE: " << res.first.second << '\n' << "ITER_COUNT: " << res.second << '\n';
			}

			else if (testStrategy == 2)
			{
				try
				{
					std::cout << "ACCURACY: ";
					std::cin >> accuracy;

					initTestFuncIA("../TestingFunctions/indexAlgorithm.txt", accuracy);
				}
				catch (bool)
				{
					std::cout << "FILE_ERROR";
				}
			}

			else
				break;
		}
	}

/*	//Task 6
	std::cout << "TASK6\n";
	std::string testExpression = "-(7/40)*(3*t+4)*sin(63/20*(t+4))";
	std::string constraint1 = "40*(cos(4*t)*(t-sin(t))*exp(-(t*t)/2))";
	std::string constraint2 = "2/25*(t+4)-sin(12/5*(t+4))";

	IndexAlgorithm task6({ testExpression, 8.86 }, { {Function(constraint1),2.0}, {Function(constraint2), 2.0 } }, -4, 4, 0.001);
	auto answer = task6.startIndexAlgorithm();

	long double expectedPoint = 2.32396;
	long double expectedValue = -1.6851399;

	std::cout << "Point: " << answer.first.first << '\n' << "Value: " << answer.first.second << '\n' << "IterCount: " << answer.second << '\n';
	std::cout << "ExpectedPoint: " << expectedPoint << '\n' << "ExpectedValue: " << expectedValue<<'\n';

	//TestTask
	std::cout << "\nTASK x^2\n";
	testExpression = "t^2";
	IndexAlgorithm testTask({ testExpression, 10 }, {}, -4, 4, 0.001);
	answer = testTask.startIndexAlgorithm();

	expectedPoint = 0;
	expectedValue = 0;

	std::cout << "Point: " << answer.first.first << '\n' << "Value: " << answer.first.second << '\n' << "IterCount: " << answer.second << '\n';
	std::cout << "ExpectedPoint: " << expectedPoint << '\n' << "ExpectedValue: " << expectedValue << '\n';
	/*
	//Task 1
	std::cout << "TASK1\n";
	testExpression = "-13/6*t+sin(13/4*(2*t+5))-53/12";
	constraint1 = "exp(-sin(3*t))-1/10*(t-1/2)*(t-1/2)-1";

	IndexAlgorithm task7(testExpression, { {Function(constraint1),2.0} }, -2.5, 1.5, 0.001);
	answer = task7.startIndexAlgorithm();

	expectedPoint = 1.25832;
	expectedValue = 4.174189;

	std::cout << "Point: " << answer.first.first << '\n' << "Value: " << answer.first.second << '\n' << "IterCount: " << answer.second << '\n';
	std::cout << "ExpectedPoint: " << expectedPoint << '\n' << "ExpectedValue: " << expectedValue;
	*/

	return 0;
}