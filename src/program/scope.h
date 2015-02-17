#ifndef SCOPE_H
#define SCOPE_H

#include <unordered_map>
#include "parser/interpreter_exception.h"
#include "program/value.h"

class Scope
{
public:
	Scope(Scope* parent);

	Value& var(const std::string& name, bool define = false);

	Scope* m_pParent;

private:
	std::unordered_map<std::string, Value> m_vars;
};

class ScopeException : public InterpreterException
{
public:
	ScopeException(int code, std::string info = "");

	std::string getErrorMessage();

	enum ErrorCodes
	{
		UNDEFINED_VARIABLE,
		UNTERMINATED_SCOPE
	};

private:
	std::string m_info;
};

#endif