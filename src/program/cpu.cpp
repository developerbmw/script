#include "program/cpu.h"
#include "program/operation.h"
#include "program.h"
#include <sstream>

CPU::CPU(Program& program) : m_program(program) { }

void CPU::process(const std::vector<Operation>& operations)
{
	for (auto& op : operations)
		process(op);
}

void CPU::process(const Operation& op)
{
	switch (op.m_opCode) {
	case OpCode::Add:
		m_regs[CPU_RESULT_REGISTER] = Value(std::to_string(m_regs[op.m_a].asLong() + m_regs[op.m_b].asLong()));
		break;

	case OpCode::Divide: {
		long long a = m_regs[op.m_a].asLong();
		long long b = m_regs[op.m_b].asLong();
		if(b == 0) {
			throw CPUException(CPUException::DIVISION_BY_ZERO);
		}
		m_regs[CPU_RESULT_REGISTER] = Value(std::to_string(a / b));
		break;
	}

	case OpCode::Subtract:
		m_regs[CPU_RESULT_REGISTER] = Value(std::to_string(m_regs[op.m_a].asLong() - m_regs[op.m_b].asLong()));
		break;

	case OpCode::Multiply:
		m_regs[CPU_RESULT_REGISTER] = Value(std::to_string(m_regs[op.m_a].asLong() * m_regs[op.m_b].asLong()));
		break;

	case OpCode::Mov:
		if (op.m_a[0] == '*') {
			if (op.m_b[0] == '*')
				m_program.scope().var(op.m_b.substr(1), true) = m_program.scope().var(op.m_a.substr(1));
			else if (op.m_b[0] == '&')
				throw CPUException(CPUException::ASSIGNMENT_TO_LITERAL);
			else
				m_regs[op.m_b] = m_program.scope().var(op.m_a.substr(1));
		} else if (op.m_a[0] == '&') {
			if (op.m_b[0] == '*')
				m_program.scope().var(op.m_b.substr(1), true) = Value(op.m_a.substr(1));
			else if (op.m_b[0] == '&')
				throw CPUException(CPUException::ASSIGNMENT_TO_LITERAL);
			else
				m_regs[op.m_b] = Value(op.m_a.substr(1));
		} else {
			if (op.m_b[0] == '*')
				m_program.scope().var(op.m_b.substr(1), true) = m_regs[op.m_a];
			else if (op.m_b[0] == '&')
				throw CPUException(CPUException::ASSIGNMENT_TO_LITERAL);
			else
				m_regs[op.m_b] = m_regs[op.m_a];
		}

		break;

	case OpCode::Push:
		m_program.stack().push_back(m_regs[op.m_a].asString());
		break;

	case OpCode::PushLiteral:
		m_program.stack().push_back(op.m_a);
		break;

	case OpCode::Pop: {
		auto val = m_program.stack().back();
		m_program.stack().pop_back();
		m_regs[op.m_a] = Value(val);
		break;
	}

	case OpCode::NewScope:
		m_program.enterNewScope();
		break;

	case OpCode::DestroyScope:
		m_program.leaveScope();
		break;

	case OpCode::Call: {
		int argCount;
		std::istringstream(m_program.stack().back()) >> argCount;
		m_program.stack().pop_back();

		auto it = m_program.m_script.m_functions.find(op.m_a);
		if(it == m_program.m_script.m_functions.end()) {
			auto it2 = m_languageFunctions.find(op.m_a);
			if(it2 == m_languageFunctions.end()) {
				throw CPUException(CPUException::UNDEFINED_FUNCTION, op.m_a);
			} else {
				std::vector<std::string> args;
				for(int i = 0; i < argCount; ++i) {
					args.push_back(m_program.stack().back());
					m_program.stack().pop_back();
				}
				it2->second->call(std::move(args));
			}
		} else {
			m_program.enterNewScope();
			for(int i = 0; i < argCount; ++i) {
				m_program.scope().var(it->second.m_arguments[i], true) = Value(m_program.stack().back());
				m_program.stack().pop_back();
			}
			process(it->second.m_operations);
			m_program.leaveScope();
		}
		break;
	}
		
	}
}

Value CPU::getReg(const std::string& reg)
{
	return m_regs[reg].asString();
}

CPUException::CPUException(int code, std::string info) : InterpreterException(code), m_info(info) { }

std::string CPUException::getErrorMessage()
{
	switch (m_code) {
	case MATH_ON_NON_INTEGER:
		return "Math on non integer";
	case DIVISION_BY_ZERO:
		return "Division by zero";
	case ASSIGNMENT_TO_LITERAL:
		return "Assignment to literal";
	case UNDEFINED_FUNCTION:
		return "Call to undefined function" + (m_info.empty() ? "" : ": " + m_info);
	default:
		return "Unknown error";
	}
}