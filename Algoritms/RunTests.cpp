#include "GlobalSearchAlgorithm.h"
#include "IndexAlgorithm.h"
#include "IndexAlgorithmMultidimessional.h"

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

void initTestFuncIA(const char fileName[], double accuracy, double coeff)
{
	std::ifstream file(fileName);

	string expression;
	int countConditions;
	vector<Function> conditionsContainer;
	long double leftBorder, rightBorder, expectedResult, expectedCoordinate;

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
			countConditions = atoi(buffer);

			for (int i = 0; i < countConditions; i++)
			{
				file.getline(buffer, bufferSize, '\n');
				string conditionsNumberI = buffer;

				conditionsContainer.push_back(Function(conditionsNumberI));
			}

			file.getline(buffer, bufferSize, '\n');
			expectedResult = atof(buffer);

			file.getline(buffer, bufferSize, '\n');
			expectedCoordinate = atof(buffer);

			IndexAlgorithm testFunc(expression, conditionsContainer, leftBorder, rightBorder, accuracy, coeff);
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

				// for GUI
				std::ofstream functionsFile("inputFile.txt");
				functionsFile.clear();

				functionsFile << expression << '\n' << leftBorder << '\n' << rightBorder << '\n' << 0 << '\n';
				functionsFile.close();
				// end GUI

				GlobalSearch testFunc(expression, leftBorder, rightBorder, accuracy, r);
				std::pair<doublePair, int> res = testFunc.searchGlobalMinimum();

				// for GUI
				std::ofstream pointFile("outputFile.txt");
				pointFile.clear();

				auto points = testFunc.getPoints();
				for (const auto& point : points)
					pointFile << point << '\n';

				pointFile.close();
				// end GUI

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
				int dimension;
				std::cout << "\nDimension = ";
				std::cin >> dimension;

				if (dimension == 1)
				{
					long double coeffForFunctions;

					std::cout << "FUNC(t): ";
					std::cin >> expression;

					std::cout << "R_COEFF: ";
					std::cin >> coeffForFunctions;

					std::cout << "LEFT_BORDER: ";
					std::cin >> leftBorder;
					std::cout << "RIGHT_BORDER: ";
					std::cin >> rightBorder;

					vector<FunctionBorder>::size_type countConditions;
					std::cout << "COUNT CONDITIONS:";
					std::cin >> countConditions;

					// for GUI
					std::ofstream functionsFile("inputFile.txt");
					functionsFile.clear();

					functionsFile << expression << '\n' << leftBorder << '\n' << rightBorder << '\n' << countConditions << '\n';
					// end GUI

					vector<Function> conditionsContainer;
					for (vector<FunctionBorder>::size_type i = 0; i < countConditions; i++)
					{
						string conditionsNumberI;

						std::cout << "CONDITION " << i << ":";
						std::cin >> conditionsNumberI;

						// for GUI
						functionsFile << conditionsNumberI << '\n';
						//end GUI

						conditionsContainer.push_back(Function(conditionsNumberI));
					}

					// for GUI
					functionsFile.close();
					// end GUI

					std::cout << "ACCURACY: ";
					std::cin >> accuracy;

					IndexAlgorithm testFunc(expression, conditionsContainer, leftBorder, rightBorder, accuracy, coeffForFunctions);
					auto res = testFunc.startIndexAlgorithm();

					// for GUI
					std::ofstream pointFile("outputFile.txt");
					pointFile.clear();

					auto points = testFunc.getPoints();
					for (const auto& point : points)
						pointFile << point << '\n';

					pointFile.close();
					// end GUI

					std::cout << "MIN VALUE: " << res.first.first << '\n' << "COORDINATE: " << res.first.second << '\n' << "ITER_COUNT: " << res.second << '\n';
				}
				//NEW
				else
				{
					Function func("- 1.5 * x * x * exp(1 - x * x - 20.25 * (x - y) * (x - y)) - ((0.5 * (x - 1) * (y - 1)) ^ 4) * exp(2 - ((0.5 * (x - 1)) ^ 4) - ((y - 1) ^ 4))", 2, "xy");
					
					std::vector<Function> constraints;
					constraints.push_back(Function("0.01 * ( ((x - 2.2) ^ 2) + ((y - 1.2) ^ 2) - 2.25)", 2, "xy"));
					constraints.push_back(Function("100 * ( 1 - ( (x - 2) ^ 2) / 1.44 - ((0.5 * y) ^ 2) )", 2, "xy"));
					constraints.push_back(Function("10 * (y - 1.5 - 1.5 * sin(6.283 * (x - 1.75)) )", 2, "xy"));

					std::vector<domain> borders;
					borders.push_back(domain(0, 4));
					borders.push_back(domain(-1, 3));

					//IndexAlgorithmMultidimessional test(Function("x^2 + y^2", 2, "xy"), {}, {domain(-2, 2), domain(-2, 2)});
					/*IndexAlgorithmMultidimessional test(Function("x^2 + y^2", 2, "xy"),
						{ Function("x + y + 0.5", 2, "xy"),
						  Function("-x - y + 0.5", 2, "xy") },
						{ domain(-2, 2), domain(-2, 2) });*/
					/*IndexAlgorithmMultidimessional test(Function("x^2 + y^2", 2, "xy"),
						{ Function("x + y + 0.5", 2, "xy") },
						{ domain(-2, 2), domain(-2, 2) });*/
					IndexAlgorithmMultidimessional test(func, constraints, borders);
					/*IndexAlgorithmMultidimessional test(Function("(x^2 + y^2) / 5", 2, "xy"),
						{ Function("0.3 - x - y", 2, "xy") },
						{ domain(-1, 1), domain(-1, 1) });*/

					auto res = test.run();
					std::cout << "MIN POINT" << "\nX = " << res.arguments[0] << "\nY = " << res.arguments[1] << '\n';
					std::cout << "MIN VALUE: " << res.value << '\n';
					std::cout << "IterCount = " << test.getIterCount() << '\n';

				}
				//END NEW
			}

			else if (testStrategy == 2)
			{
				try
				{
					double coeff;

					std::cout << "R_COEFF: ";
					std::cin >> coeff;

					std::cout << "ACCURACY: ";
					std::cin >> accuracy;

					initTestFuncIA("../TestingFunctions/indexAlgorithm.txt", accuracy, coeff);
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

	return 0;
}