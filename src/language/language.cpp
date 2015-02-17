#include "language/language.h"
#include "program/cpu.h"
#include "language/language_functions.h"

Language::Language()
{
	m_openParentheses = "(";
	m_closeParentheses = ")";
	m_openBlock = "{";
	m_closeBlock = "}";
	m_endLine = ";";
	m_argumentSeparator = ",";
	m_operators.push_back(&m_operatorEquals);
	m_operators.push_back(&m_operatorPlus);
	m_operators.push_back(&m_operatorHyphen);
	m_operators.push_back(&m_operatorAsterisk);
	m_operators.push_back(&m_operatorForwardSlash);
}

SITokenType Language::tokenType(const std::string& tokenStr)
{
	if(tokenStr[0] == '"' && tokenStr[tokenStr.length() - 1] == '"') {
		return SITokenType::StringLiteral;
	}

	bool number = true;
	bool decimal = false;
	for(auto& c : tokenStr) {
		if((c < '0' || c > '9')) {
			if(c == '.' && !decimal) {
				decimal = true;
			} else {
				number = false;
				break;
			}
		}
	}
	if(number) {
		if(decimal) {
			return SITokenType::DecimalLiteral;
		} else {
			return SITokenType::IntegerLiteral;
		}
	}

	for (auto& c : tokenStr)
		if ((c < 'A' || c > 'z') && (c < '0' || c > '9') && c != '_')
			return SITokenType::Invalid;

	return SITokenType::Identifier;
}

void Language::registerFunctions(CPU& c)
{
	c.registerFunction<LFEcho>("echo");
	c.registerFunction<LFEchoLine>("echo_line");
	c.registerFunction<LFMsgbox>("msgbox");
}