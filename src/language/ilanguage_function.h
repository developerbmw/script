#ifndef ILANGUAGE_FUNCTION_H
#define ILANGUAGE_FUNCTION_H

#include <vector>
#include <string>

class ILanguageFunction
{
public:
	ILanguageFunction() { }

	virtual void call(std::vector<std::string> args) = 0;
};

class LanguageFunctionException
{
public:
	LanguageFunctionException(std::string msg) : m_msg(msg) { }

	std::string& getErrorMessage() { return m_msg; }

private:
	std::string m_msg;
};

#endif