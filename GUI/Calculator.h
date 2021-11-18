#pragma once

#include <string>


class Calculator
{
	std::string infixExpression;
	std::string postfixExpression;
	bool flagNewExpression;
	long double lastResult;

	int getPriority(char symbol) const;

	bool checkCorrectnessExpression() const;
	bool openingBrackets(char symbol) const;
	bool beginComplexFunction(char symbol) const;

	bool configureExpression(const std::string& basicExpression);
	void infixToPostfix();
public:
	Calculator();
	Calculator(const std::string& basicExpression);
	void replaceExpression(const std::string& basicExpression);
	long double calculateResult();
};

