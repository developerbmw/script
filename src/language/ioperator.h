#ifndef IOPERATOR_H
#define IOPERATOR_H

#include <string>
#include <vector>
#include "program/operation.h"
#include "parser/token.h"

class Script;

class IOperator
{
public:
	IOperator(std::string code, int priority, bool reverse) : m_code(code), m_priority(priority), m_reverse(reverse) { }
	virtual ~IOperator() { }

	virtual void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd, std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script) { }

	bool operator<(const IOperator& other) { return m_priority < other.m_priority; }

	// This is the code the programmer will use for this operator e.g. +
	std::string m_code;

	// Lower priority means it is processed later.
	// For example + and - have a lower number than * and /
	int m_priority;

	// If true, the parser will do a reverse search (from end to start) for the operator
	bool m_reverse;
};

#endif