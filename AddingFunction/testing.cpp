#include "Calculator.h"

int main()
{
	string expression;

	while(expression != "F")
	{
		std::cout << "EXPRESSION: ";
		getline(std::cin, expression);

		Calculator calculator(expression);

	    calculator.insertNewExpression(expression);
		calculator.calculateResult();

		std::cout << "RESULT: " << calculator.calculateResult() << '\n' << "PRESS F FOR EXIT" << '\n';
	}

	return 0;
}