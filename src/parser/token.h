#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum SITokenType
{
	Invalid,
	Identifier,
	IntegerLiteral,
	DecimalLiteral,
	StringLiteral,
	Operator,
	Structure
};

class Token
{
public:
	Token(std::string value, SITokenType type, int pLevel = 0);

	bool operator==(const std::string& other) const;
	bool operator!=(const std::string& other) const;

	std::string m_value;
	SITokenType m_type;
	int m_pLevel;
};

#endif