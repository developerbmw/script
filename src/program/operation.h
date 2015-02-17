#ifndef OPERATION_H
#define OPERATION_H

#include <string>

enum OpCode
{
	Mov,
	Add,
	Subtract,
	Multiply,
	Divide,
	Push,
	Pop,
	NewScope,
	DestroyScope,
	Call,
	PushLiteral
};

class Operation
{
public:
	Operation(OpCode opCode, std::string a = "", std::string b = "");

	std::string toString();

	OpCode m_opCode;
	std::string m_a;
	std::string m_b;
};

#endif