#include "parser/script.h"
#include "util/util.h"
#include "language/ioperator.h"
#include <algorithm>
#include "program/cpu.h"

Script::Script(std::string script, ILanguage& language) : m_pLanguage(&language)
{
	tokenize(script.begin(), script.end());

	// pre-processing
	int pLevel = 0;
	SITokenType prevType = SITokenType::Invalid;
	bool function = false;

	for (auto it = m_tokens.begin(); it != m_tokens.end();) {
		if (*it == m_pLanguage->m_openParentheses) {
			++pLevel;

			if (prevType != SITokenType::Identifier) {
				prevType = it->m_type;
				it = m_tokens.erase(it);
			} else {
				function = true;
				prevType = it->m_type;
				++it;
			}
		} else if(*it == m_pLanguage->m_closeParentheses) {
			if(pLevel-- <= 0)
				throw ParserException(ParserException::SYNTAX_ERROR, m_pLanguage->m_closeParentheses);

			if(function) {
				prevType = it->m_type;
				++it;
				function = false;
			} else {
				prevType = it->m_type;
				it = m_tokens.erase(it);
			}
		} else {
			it->m_pLevel = pLevel;
			prevType = it->m_type;
			++it;
		}
	}

	if (pLevel != 0)
		throw ParserException(ParserException::UNTERMINATED_PARENTHESES);

	// parse
	m_outputStack.push_front(&m_operations);
	parse(m_tokens.begin(), m_tokens.end());
}

void Script::tokenize(std::string::iterator start, std::string::iterator end)
{
	auto pos = std::search(start, end, m_pLanguage->m_endLine.begin(), m_pLanguage->m_endLine.end());
	if (pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_endLine, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	pos = std::search(start, end, m_pLanguage->m_argumentSeparator.begin(), m_pLanguage->m_argumentSeparator.end());
	if(pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_argumentSeparator, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	pos = std::search(start, end, m_pLanguage->m_openParentheses.begin(), m_pLanguage->m_openParentheses.end());
	if (pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_openParentheses, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	pos = std::search(start, end, m_pLanguage->m_closeParentheses.begin(), m_pLanguage->m_closeParentheses.end());
	if (pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_closeParentheses, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	pos = std::search(start, end, m_pLanguage->m_openBlock.begin(), m_pLanguage->m_openBlock.end());
	if (pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_openBlock, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	pos = std::search(start, end, m_pLanguage->m_closeBlock.begin(), m_pLanguage->m_closeBlock.end());
	if (pos != end) {
		tokenize(start, pos);
		m_tokens.emplace_back(m_pLanguage->m_closeBlock, SITokenType::Structure);
		tokenize(pos + 1, end);
		return;
	}

	for (auto& op : m_pLanguage->m_operators) {
		pos = std::search(start, end, op->m_code.begin(), op->m_code.end());
		if (pos != end) {
			tokenize(start, pos);
			m_tokens.emplace_back(op->m_code, SITokenType::Operator);
			tokenize(pos + 1, end);
			return;
		}
	}

	std::string str(start, end);
	Util::trim(str);

	if (str.empty())
		return;

	SITokenType type = m_pLanguage->tokenType(str);

	if (type == SITokenType::Invalid)
		throw ParserException(ParserException::INVALID_TOKEN, str);

	m_tokens.emplace_back(str, type);
}

void Script::parse(std::vector<Token>::iterator start, std::vector<Token>::iterator end)
{
	auto pos = start;
	int blockLevel = 0;
	for(auto it = start; it != end; ++it) {
		if((blockLevel == 0) && (*it == m_pLanguage->m_endLine)) {
			parseExpression(pos, it);
			pos = it + 1;
		} else if(*it == m_pLanguage->m_openBlock) {
			++blockLevel;
		} else if(*it == m_pLanguage->m_closeBlock) {
			--blockLevel;
			parseExpression(pos, it + 1);
			pos = it + 1;
		}
	}
}

void Script::parseExpression(std::vector<Token>::iterator start, std::vector<Token>::iterator end)
{
	if (end - start == 0)
		return;

	// process function definitions
	auto identifier = end;
	auto openParentheses = start;
	auto closeParentheses = start;
	auto openBlock = start;
	auto closeBlock = start;
	for(auto it = start; it != end; ++it) {
		if((it->m_type == SITokenType::Identifier) && (identifier == end)) {
			identifier = it;
		} else if((*it == m_pLanguage->m_openParentheses) && (openParentheses == start)) {
			openParentheses = it;
		} else if((*it == m_pLanguage->m_closeParentheses) && (closeParentheses == start)) {
			closeParentheses = it;
		} else if((*it == m_pLanguage->m_openBlock) && (openBlock == start)) {
			openBlock = it;
		} else if((*it == m_pLanguage->m_closeBlock) && (closeBlock == start)) {
			closeBlock = it;
		}
	}
	if((openParentheses - identifier == 1) && (openBlock != start)) {
		if(closeParentheses == start) {
			throw ParserException(ParserException::SYNTAX_ERROR);
		}

		auto argsStart = openParentheses + 1;
		auto argsEnd = closeParentheses;
		std::vector<std::string> arguments;
		for(auto it = argsStart; it != argsEnd; ++it) {
			arguments.push_back(it->m_value);
		}
		auto p = m_functions.emplace(identifier->m_value, arguments);
		m_outputStack.push_front(&p.first->second.m_operations);
		parse(openBlock + 1, closeBlock);
		m_outputStack.pop_front();
		return;
	}

	// process scopes
	for (auto it = start; it != end; ++it) {
		if (*it == m_pLanguage->m_openBlock) {
			parseExpression(start, it);
			m_outputStack.front()->emplace_back(OpCode::NewScope);
			parseExpression(it + 1, end);
			return;
		} else if (*it == m_pLanguage->m_closeBlock) {
			parseExpression(start, it);
			m_outputStack.front()->emplace_back(OpCode::DestroyScope);
			parseExpression(it + 1, end);
			return;
		}
	}

	// process single token expressions
	if (end - start == 1) {
		if(start->m_type == SITokenType::Identifier) {
			m_outputStack.front()->emplace_back(OpCode::Mov, "*" + start->m_value, CPU_RESULT_REGISTER);
		} else if(start->m_type == SITokenType::IntegerLiteral || start->m_type == SITokenType::DecimalLiteral) {
			m_outputStack.front()->emplace_back(OpCode::Mov, "&" + start->m_value, CPU_RESULT_REGISTER);
		} else if(start->m_type == SITokenType::StringLiteral) {
			m_outputStack.front()->emplace_back(OpCode::Mov, "&" + std::string(start->m_value.begin() + 1, start->m_value.end() - 1), CPU_RESULT_REGISTER);
		} else {
			throw ParserException(ParserException::INVALID_EXPRESSION);
		}

		return;
	}

	// process operators
	for (auto& op : m_pLanguage->m_operators) {
		if (op->m_reverse) {
			std::reverse(start, end);
			for (auto it = start; it != end; ++it) {
				if (it->m_value == op->m_code && it->m_pLevel == 0) {
					std::reverse(it + 1, end);
					std::reverse(start, it);
					op->parse(it + 1, end, start, it, *this);
					return;
				}
			}
		} else {
			for (auto it = start; it != end; ++it) {
				if (it->m_value == op->m_code && it->m_pLevel == 0) {
					op->parse(start, it, it + 1, end, *this);
					return;
				}
			}
		}
	}

	// process function calls
	identifier = end;
	auto parentheses = start;

	for (auto it = start; it != end; ++it) {
		if (it->m_type == SITokenType::Identifier)
			identifier = it;
		else if (*it == m_pLanguage->m_openParentheses)
			parentheses = it;

		if (parentheses - identifier == 1) {
			if (*(end - 1) != m_pLanguage->m_closeParentheses)
				throw ParserException(ParserException::SYNTAX_ERROR);
			// push args onto stack in reverse order
			auto pos = end - 1;
			auto first = parentheses + 1;
			int count = 0;
			for(auto it = pos - 1; it != first - 1; --it) {
				if(*it == m_pLanguage->m_argumentSeparator) {
					parseExpression(it + 1, pos);
					m_outputStack.front()->emplace_back(OpCode::Push, CPU_RESULT_REGISTER);
					pos = it;
					++count;
				} else if(it == first) {
					parseExpression(first, pos);
					m_outputStack.front()->emplace_back(OpCode::Push, CPU_RESULT_REGISTER);
					++count;
				}
			}
			// push arg count
			m_outputStack.front()->emplace_back(OpCode::PushLiteral, std::to_string(count));
			// call
			m_outputStack.front()->emplace_back(OpCode::Call, identifier->m_value, "");
			return;
		}
	}

	// anything left over should be ones with pLevel more than 0
	for (auto it = start; it != end; ++it) {
		if (it->m_pLevel <= 0)
			throw ParserException(ParserException::INVALID_EXPRESSION);

		--it->m_pLevel;
	}

	parseExpression(start, end);
}

void Script::appendOperation(Operation&& operation)
{
	m_outputStack.front()->push_back(operation);
}

ParserException::ParserException(int code, std::string info) : InterpreterException(code), m_info(info) { }

std::string ParserException::getErrorMessage()
{
	std::string msg;

	switch (m_code) {
	case SYNTAX_ERROR:
		msg = "Syntax error";
		break;
	case UNTERMINATED_PARENTHESES:
		msg = "Unterminated parentheses";
		break;
	case UNTERMINATED_SCOPE:
		msg = "Unterminated scope";
		break;
	case INVALID_TOKEN:
		msg = "Invalid token";
		break;
	case INVALID_EXPRESSION:
		msg = "Invalid expression";
		break;
	case UNTERMINATED_FUNCTION:
		msg = "Unterminated function definition";
		break;
	default:
		msg = "Unknown error";
		break;
	}

	if (!m_info.empty())
		msg += ": " + m_info;

	return msg;
}