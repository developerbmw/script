#ifndef SCRIPT_H
#define SCRIPT_H

#include <istream>
#include <vector>
#include "language/ilanguage.h"
#include "program/interpreter_exception.h"
#include "program/operation.h"
#include "program/function.h"
#include <unordered_map>
#include <forward_list>

class Script
{
	friend class Program;
public:
	Script(std::string script, ILanguage& language);

	void tokenize(std::string::iterator start, std::string::iterator end);
	void parse(std::vector<Token>::iterator start, std::vector<Token>::iterator end);
	void parseExpression(std::vector<Token>::iterator start, std::vector<Token>::iterator end);
	void appendOperation(Operation&& operation);

	std::unordered_map<std::string, Function> m_functions;

protected:
	std::vector<Operation> m_operations;

private:
	std::vector<Token> m_tokens;
	ILanguage* m_pLanguage;
	std::forward_list<std::vector<Operation>*> m_outputStack;
};

class ParserException : public InterpreterException
{
public:
	ParserException(int code, std::string info = "");

	std::string getErrorMessage();

	enum ErrorCodes
	{
		SYNTAX_ERROR,
		UNTERMINATED_PARENTHESES,
		UNTERMINATED_SCOPE,
		INVALID_TOKEN,
		INVALID_EXPRESSION,
		UNTERMINATED_FUNCTION
	};

private:
	std::string m_info;
};

#endif