#ifndef CPU_H
#define CPU_H

#include <string>
#include <unordered_map>
#include "Operation.h"
#include "program/interpreter_exception.h"
#include "language/ilanguage_function.h"
#include "program/value.h"

#define CPU_RESULT_REGISTER "c"
#define CPU_GENERAL_REG_1 "a"
#define CPU_GENERAL_REG_2 "b"

#include <memory>

class Program;

class CPU
{
public:
	CPU(Program& program);

	void process(const std::vector<Operation>& operations);
	void process(const Operation& op);
	Value getReg(const std::string& reg);

	template <class T>
	void registerFunction(std::string name)
	{
		m_languageFunctions[name] = std::make_unique<T>();
	}

private:
	std::unordered_map<std::string, Value> m_regs;
	Program& m_program;
	std::unordered_map<std::string, std::unique_ptr<ILanguageFunction>> m_languageFunctions;
};

class CPUException : public InterpreterException
{
public:
	CPUException(int code, std::string info = "");

	std::string getErrorMessage();

	std::string m_info;

	enum ErrorCodes
	{
		MATH_ON_NON_INTEGER,
		DIVISION_BY_ZERO,
		ASSIGNMENT_TO_LITERAL,
		UNDEFINED_FUNCTION
	};
};

#endif