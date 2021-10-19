#pragma once

#include <iostream>
#include <stack>
#include <string>

using std::string;

class Calculator
{
	string infixExpression;
	string postfixExpression;
	bool flagNewExpression;
	long double lastResult;

	int getPriority(char symbol) const;

	bool checkCorrectnessExpression() const;
	bool openingBrackets(char symbol) const;
	bool beginComplexFunction(char symbol) const;

	bool configureExpression(const string& basicExpression);
	void infixToPostfix();
public:
	Calculator();
	Calculator(const string& basicExpression);
	void replaceExpression(const string& basicExpression);
	long double calculateResult();
};

