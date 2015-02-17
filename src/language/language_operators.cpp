#include "language/language_operators.h"
#include "parser/script.h"
#include "program/cpu.h"

void TLOEquals::parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script)
{
	script.parseExpression(afterStart, afterEnd);

	if (beforeEnd - beforeStart < 1 || beforeEnd - beforeStart > 1 || (beforeEnd - 1)->m_type != SITokenType::Identifier)
		throw ParserException(ParserException::INVALID_EXPRESSION);

	script.appendOperation(Operation(OpCode::Mov, CPU_RESULT_REGISTER, "*" + (beforeEnd - 1)->m_value));
}

void TLOPlus::parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script)
{
	script.parseExpression(beforeStart, beforeEnd);
	script.appendOperation(Operation(OpCode::Push, CPU_GENERAL_REG_1));
	script.appendOperation(Operation(OpCode::Mov, CPU_RESULT_REGISTER, CPU_GENERAL_REG_1));
	script.parseExpression(afterStart, afterEnd);
	script.appendOperation(Operation(OpCode::Add, CPU_GENERAL_REG_1, CPU_RESULT_REGISTER));
	script.appendOperation(Operation(OpCode::Pop, CPU_GENERAL_REG_1));
}

void TLOHyphen::parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script)
{
	script.parseExpression(beforeStart, beforeEnd);
	script.appendOperation(Operation(OpCode::Push, CPU_GENERAL_REG_1));
	script.appendOperation(Operation(OpCode::Mov, CPU_RESULT_REGISTER, CPU_GENERAL_REG_1));
	script.parseExpression(afterStart, afterEnd);
	script.appendOperation(Operation(OpCode::Subtract, CPU_GENERAL_REG_1, CPU_RESULT_REGISTER));
	script.appendOperation(Operation(OpCode::Pop, CPU_GENERAL_REG_1));
}

void TLOAsterisk::parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script)
{
	script.parseExpression(beforeStart, beforeEnd);
	script.appendOperation(Operation(OpCode::Push, CPU_GENERAL_REG_1));
	script.appendOperation(Operation(OpCode::Mov, CPU_RESULT_REGISTER, CPU_GENERAL_REG_1));
	script.parseExpression(afterStart, afterEnd);
	script.appendOperation(Operation(OpCode::Multiply, CPU_GENERAL_REG_1, CPU_RESULT_REGISTER));
	script.appendOperation(Operation(OpCode::Pop, CPU_GENERAL_REG_1));
}

void TLOForwardSlash::parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script)
{
	script.parseExpression(beforeStart, beforeEnd);
	script.appendOperation(Operation(OpCode::Push, CPU_GENERAL_REG_1));
	script.appendOperation(Operation(OpCode::Mov, CPU_RESULT_REGISTER, CPU_GENERAL_REG_1));
	script.parseExpression(afterStart, afterEnd);
	script.appendOperation(Operation(OpCode::Divide, CPU_GENERAL_REG_1, CPU_RESULT_REGISTER));
	script.appendOperation(Operation(OpCode::Pop, CPU_GENERAL_REG_1));
}