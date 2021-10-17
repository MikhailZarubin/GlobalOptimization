#include "Calculator.h"

int main()
{
	string expression;
	Calculator calculator;

	while(true)
	{
		std::cout << "EXPRESSION: ";
		getline(std::cin, expression);

		calculator.insertNewExpression(expression);

		std::cout << "RESULT: " << calculator.calculateResult() << '\n';
	}

	return 0;
}