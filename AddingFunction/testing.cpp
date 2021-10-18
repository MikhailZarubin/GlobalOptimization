#include "Calculator.h"
#include "Function.h"

int main()
{
	double leftBorder, rightBorder, step;

	string expression;

	while(true)
	{
		std::cout << "FUNC(t): ";
		std::cin >> expression;

		std::cout << "LEFT_BORDER: ";
		std::cin >> leftBorder;
		std::cout << "RIGHT_BORDER: ";
		std::cin >> rightBorder;

		Function func(expression, leftBorder, rightBorder);

		std::cout << "STEP: ";
		std::cin >> step;

		for (double i = leftBorder; i <= rightBorder; i += step)
			std::cout << "FUNC (" << i << ") = " << func.getValue(i) << '\n';
	}

	return 0;
}