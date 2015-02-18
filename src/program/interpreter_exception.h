#ifndef INTERPRETER_EXCEPTION_H
#define INTERPRETER_EXCEPTION_H

class InterpreterException
{
public:
	InterpreterException(int code) : m_code(code) { }
	virtual ~InterpreterException() { }

	virtual std::string getErrorMessage() { return 0; }

	int m_code;
};

#endif