#ifndef ILANGUAGE_H
#define ILANGUAGE_H

#include <vector>
#include <string>
#include "parser/token.h"

class IOperator;
class CPU;

class ILanguage
{
public:
	virtual ~ILanguage() { }

	// any tokens that are not operators or parentheses will be sent
	// to this function for classification
	virtual SITokenType tokenType(const std::string& tokenStr) = 0;
	virtual void registerFunctions(CPU& c) { };

	std::string m_openParentheses;
	std::string m_closeParentheses;
	std::string m_openBlock;
	std::string m_closeBlock;
	std::string m_endLine;
	std::string m_argumentSeparator;
	std::vector<IOperator*> m_operators;
};

#endif