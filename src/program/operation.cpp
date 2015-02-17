#include "program/operation.h"

Operation::Operation(OpCode opCode, std::string a, std::string b) : m_opCode(opCode), m_a(a), m_b(b) { }

std::string Operation::toString()
{
	std::string str;

	switch (m_opCode) {
	case OpCode::Add:
		str = "Add " + m_a + ", " + m_b;
		break;
	case OpCode::Divide:
		str = "Div " + m_a + ", " + m_b;
		break;
	case OpCode::Mov:
		str = "Mov " + m_a + ", " + m_b;
		break;
	case OpCode::Multiply:
		str = "Mul " + m_a + ", " + m_b;
		break;
	case OpCode::Pop:
		str = "Pop " + m_a;
		break;
	case OpCode::Push:
		str = "Push " + m_a;
		break;
	case OpCode::Subtract:
		str = "Sub " + m_a + ", " + m_b;
		break;
	case OpCode::NewScope:
		str = "NewScope";
		break;
	case OpCode::DestroyScope:
		str = "DestroyScope";
		break;

	case OpCode::Call:
		str = "Call " + m_a;
		break;

	case OpCode::PushLiteral:
		str = "PushLiteral " + m_a;
		break;

	default:
		str = "Unknown " + m_a + ", " + m_b;
		break;
	}

	return str;
}