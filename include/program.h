#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <vector>
#include <forward_list>
#include "program/cpu.h"
#include "program/scope.h"
#include "parser/script.h"

class ILanguage;

class Program
{
	friend class CPU;
public:
	Program(std::istream& source);

	void run(std::ostream& out);
	Scope& scope();
	std::vector<std::string>& stack();
	void enterNewScope();
	void leaveScope(bool end = false);

private:
	CPU m_cpu;
	Scope* m_currentScope;
	std::unique_ptr<ILanguage> m_language;

protected:
	Script m_script;

private:
	std::forward_list<Scope> m_scopes;
	std::vector<std::string> m_stack;
};

#endif