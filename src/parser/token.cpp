#include "parser/token.h"

Token::Token(std::string value, SITokenType type, int pLevel) : m_value(value), m_type(type), m_pLevel(pLevel) { }

bool Token::operator==(const std::string& other) const
{
	return m_value == other;
}

bool Token::operator!=(const std::string& other) const
{
	return m_value != other;
}