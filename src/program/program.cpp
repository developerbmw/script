#include "program.h"
#include "language/language.h"
#include "util/util.h"
#include <algorithm>

Program::Program(std::istream& source) : m_cpu(*this), m_currentScope(nullptr), m_language(std::make_unique<Language>()),
	m_script(std::string(std::istreambuf_iterator<char>(source), std::istreambuf_iterator<char>()), *m_language)
{
	m_language->registerFunctions(m_cpu);
}

void Program::run(std::ostream& out)
{
	try {
		enterNewScope();
		m_cpu.process(m_script.m_operations);
		leaveScope(true);
		if (m_currentScope != nullptr)
			throw ScopeException(ScopeException::UNTERMINATED_SCOPE);
	} catch (InterpreterException& e) {
		out << "Error - " << e.getErrorMessage() << std::endl;
	} catch(LanguageFunctionException& e) {
		out << "Error - " << e.getErrorMessage() << std::endl;
	}
}

Scope& Program::scope()
{
	return *m_currentScope;
}

std::vector<std::string>& Program::stack()
{
	return m_stack;
}

void Program::enterNewScope()
{
	m_scopes.emplace_front(m_currentScope);
	m_currentScope = &m_scopes.front();
}

void Program::leaveScope(bool end)
{
	if (!end && m_currentScope->m_pParent == nullptr)
		throw ScopeException(ScopeException::UNTERMINATED_SCOPE);
	m_currentScope = m_scopes.front().m_pParent;
	m_scopes.pop_front();
}