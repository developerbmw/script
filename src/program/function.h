#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include "program/operation.h"

class Function
{
public:
	Function(std::vector<std::string> arguments) : m_arguments(arguments) { }

	std::vector<std::string> m_arguments;
	std::vector<Operation> m_operations;
};

#endif