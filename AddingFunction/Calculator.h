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
	double lastResult;

	int getPriority(char symbol) const;

	bool checkCorrectnessExpression() const;
	bool openingBrackets(char symbol) const;
	bool beginComplexFunction(char symbol) const;

	bool configureExpression(const string& basicExpression);
	void infixToPostfix();
public:
	Calculator(string basicExpression = "2+3");
	void insertNewExpression(string basicExpression);
	double calculateResult();
};

