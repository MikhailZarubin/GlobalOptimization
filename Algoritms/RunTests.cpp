#include "GlobalSearchAlgorithm.h"
#include "IndexAlgorithm.h"

#include <iostream>
#include <fstream>

using std::string;


int main()
{
	/*int testStrategy{0};

	double leftBorder, rightBorder, accuracy, r;
	string expression;

	while (testStrategy < 3)
	{
		std::cout << "\n1. Manual input\n2. Testing Functions\n3. Exit\n";
		std::cin >> testStrategy;

		if (testStrategy == 1)
		{

			while (true)
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
		}
		else
		{
			const int bufferSize = 150;
			char buffer[bufferSize];
			std::ifstream file("../TestingFunctions/globalSearchAlgorithm.txt");

			double expectedResult, expectedCoordinate;

			std::cout << "ACCURACY: ";
			std::cin >> accuracy;

			std::cout << "R_COEFF: ";
			std::cin >> r;

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

					GlobalSearch testFunc(expression, leftBorder, rightBorder, accuracy, r);
					std::pair<doublePair, int> res = testFunc.searchGlobalMinimum();

					std::cout << "\nFUNC: " << expression << "\nLEFT_BORDER: " << leftBorder << "\nRIGHT_BORDER: " << rightBorder << "\nACCURACY: " << accuracy << "\nR_COEFF: " << r
						<< "\nMIN VALUE: " << res.first.first << "\nCOORDINATE: " << res.first.second << "\nITER_COUNT: " << res.second
						<< "\nEXPECTED_RESULT: " << expectedResult << "\nEXPECTED_COORDINATE: " << expectedCoordinate << "\nERROR: " << fabsl(res.first.first - expectedResult) << '\n';

				}
				file.close();
			}
			else
				std::cout << "FILE ERROR\n";
		}
	}*/

	//Task 6
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