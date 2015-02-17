#include "program/scope.h"

Scope::Scope(Scope* parent) : m_pParent(parent) { }

Value& Scope::var(const std::string& name, bool define)
{
	Scope* scope = this;

	do {
		auto it = scope->m_vars.find(name);
		if (it != scope->m_vars.end())
			return it->second;

		scope = scope->m_pParent;
	} while (scope != nullptr);

	if (define)
		return m_vars[name];

	throw ScopeException(ScopeException::UNDEFINED_VARIABLE, name);
}

ScopeException::ScopeException(int code, std::string info) : InterpreterException(code), m_info(info) { }

std::string ScopeException::getErrorMessage()
{
	std::string msg;

	switch (m_code) {
	case UNDEFINED_VARIABLE:
		msg = "Undefined variable";
		break;
	case UNTERMINATED_SCOPE:
		msg = "Unterminated scope";
		break;
	default:
		msg = "Unknown error";
		break;
	}

	if (!m_info.empty())
		msg += ": " + m_info;

	return msg;
}