#include "Calculator.h"

using std::stack;

namespace
{
	string defaultExpression = "5*(4-2)";
	const char delimiter = '|';
	int DELIMITER_PRIORITY = -1; // |
	int NULL_PRIORITY = 0; // number
	int LOW_PRIORITY = 1; // brackets and beginnings of complex functions
	int MEDIUM_PRIORITY = 2; // + and -
	int HIGH_PRIORITY = 3; // * and /
	int HIGHEST_PRIORITY = 4; // ^
}


int Calculator::getPriority(char symbol) const
{
	switch (symbol)
	{
	case'+': return ::MEDIUM_PRIORITY;
	case'-': return ::MEDIUM_PRIORITY;
	case'*': return ::HIGH_PRIORITY;
	case'/': return ::HIGH_PRIORITY;
	case'(':return ::LOW_PRIORITY;
	case')':return ::LOW_PRIORITY;
	case'^':return ::HIGHEST_PRIORITY;
	case's':return ::LOW_PRIORITY;
	case'c':return ::LOW_PRIORITY;
	case'l':return ::LOW_PRIORITY;
	case'e':return ::LOW_PRIORITY; 
	case ::delimiter: return ::DELIMITER_PRIORITY;
	default: return ::NULL_PRIORITY;
	}
}


bool Calculator::beginComplexFunction(char symbol) const
{
	return symbol == 's' || symbol == 'c' || symbol == 'e' || symbol == 'l';
}


bool Calculator::openingBrackets(char symbol) const
{
	return symbol == '(' || beginComplexFunction(symbol);
}


Calculator::Calculator() : Calculator(::defaultExpression) {}


Calculator::Calculator(const string& basicExpression) : infixExpression{}, postfixExpression{}, flagNewExpression{ true }, lastResult{ 0. }
{
	bool correct = configureExpression(basicExpression);

	if (!correct)
		throw 0;
}


bool Calculator::configureExpression(const string& basicExpression)
{
	infixExpression = "(";

	for (string::size_type i = 0; i < basicExpression.size(); i++)
	{
		if (beginComplexFunction(basicExpression[i]))
		{
			infixExpression += basicExpression[i];
			i += 3;

			if (i >= basicExpression.size() - 2)
				throw 0;
		}

		else if (basicExpression[i] != ' ')
		{
			string::size_type size = infixExpression.size();

			if ((size == 0 || (size > 0 && getPriority(infixExpression[size - 1]) == ::LOW_PRIORITY)) && basicExpression[i] == '-')
				infixExpression += '0';

			infixExpression += basicExpression[i];
		}
	}

	infixExpression += ")";

	return checkCorrectnessExpression();
}


bool Calculator::checkCorrectnessExpression() const
{
	int countBrackets{ 0 };

	for (const auto& symbol: infixExpression)
	{
		if (openingBrackets(symbol))
			countBrackets++;

		else if (symbol == ')')
		{
			if (countBrackets == 0)
				return false;

			countBrackets--;
		}
	}
	return countBrackets == 0;
}


void Calculator::infixToPostfix()
{
	stack <char> operationsStack;
	char topStack;

	postfixExpression.clear();

	for (const auto& symbol: infixExpression)
	{
		if (getPriority(symbol) != ::NULL_PRIORITY && !postfixExpression.empty() && getPriority(postfixExpression[postfixExpression.size() - 1]) == ::NULL_PRIORITY)
			postfixExpression += ::delimiter;

		if (getPriority(symbol) == ::NULL_PRIORITY)
			postfixExpression += symbol;

		else if (openingBrackets(symbol))
			operationsStack.push(symbol);

		else if (symbol == ')')
		{
			topStack = operationsStack.top();

			while (!openingBrackets(topStack))
			{
				postfixExpression += topStack;

				operationsStack.pop();
				topStack = operationsStack.top();
			}

			operationsStack.pop();

			if (topStack != '(')
				postfixExpression += topStack;
		}

		else
		{
			topStack = operationsStack.top();

			while (getPriority(topStack) >= getPriority(symbol))
			{
				postfixExpression += topStack;

				operationsStack.pop();
				topStack = operationsStack.top();
			}

			operationsStack.push(symbol);
		}
	}

	if (!operationsStack.empty())
		throw 0;
}


double Calculator::calculateResult()
{
	if (!flagNewExpression)
		return lastResult;

	infixToPostfix();

	stack <double> valuesStack;

	double a, b;
	string number;

	for (string::size_type i = 0; i < postfixExpression.size(); i++)
	{
		switch (postfixExpression[i])
		{

		case'+':
			a = valuesStack.top(); valuesStack.pop();
			b = valuesStack.top();
			valuesStack.top() = b + a;
			break;

		case'-':
			a = valuesStack.top(); valuesStack.pop();
			b = valuesStack.top();
			valuesStack.top() = b - a;
			break;

		case'*':
			a = valuesStack.top(); valuesStack.pop();
			b = valuesStack.top();
			valuesStack.top() = b * a;
			break;

		case'/':
			a = valuesStack.top(); valuesStack.pop();
			b = valuesStack.top();
			valuesStack.top() = b / a;
			break;

		case'^':
			a = valuesStack.top(); valuesStack.pop();
			b = valuesStack.top();
			valuesStack.top() = pow(b, a);
			break;

		case's':
			a = valuesStack.top();
			valuesStack.top() = sin(a);
			break;

		case'c':
			a = valuesStack.top();
			valuesStack.top() = cos(a);
			break;

		case'e':
			a = valuesStack.top();
			valuesStack.top() = exp(a);
			break;

		case'l':
			a = valuesStack.top();
			valuesStack.top() = log(a);
			break;

		default:

			while (i < postfixExpression.size() && getPriority(postfixExpression[i]) == 0 && postfixExpression[i] != ::delimiter)
				number += postfixExpression[i++];

			valuesStack.push(std::stod(number));
			number.clear();

			break;
		}
	}

	if (valuesStack.size() != 1)
		throw 0;

	flagNewExpression = false;
	lastResult = valuesStack.top();

	return lastResult;
}


void Calculator::replaceExpression(const string& basicExpression)
{
	flagNewExpression = true;
	bool correct = configureExpression(basicExpression);

	if (!correct)
		throw 0;
}