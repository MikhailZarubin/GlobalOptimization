#include "Calculator.h"
#include "Function.h"

#include <fstream>

int main()
{
	int testStrategy{ 0 };

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

				GlobalMinimumFunction testFunc(expression, leftBorder, rightBorder, accuracy, r);
				std::pair<doublePair, int> res = testFunc.searchGlobalMinimum();

				std::cout << "MIN VALUE: " << res.first.first << '\n' << "COORDINATE: " << res.first.second << '\n' << "ITER_COUNT: " << res.second << '\n';
			}
		}
		else
		{
			const int bufferSize = 150;
			char buffer[bufferSize];
			std::ifstream file("testingFunctions.txt");

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

					GlobalMinimumFunction testFunc(expression, leftBorder, rightBorder, accuracy, r);
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
	}
	return 0;
}