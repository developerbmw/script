#ifndef LANGUAGE_OPERATORS_H
#define LANGUAGE_OPERATORS_H

#include "language/ioperator.h"

class Script;

class TLOEquals : public IOperator
{
public:
	TLOEquals() : IOperator("=", 1, false) { }

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

class TLOPlus : public IOperator
{
public:
	TLOPlus() : IOperator("+", 2, true) { }

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

class TLOHyphen : public IOperator
{
public:
	TLOHyphen() : IOperator("-", 2, true) { }

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

class TLOAsterisk : public IOperator
{
public:
	TLOAsterisk() : IOperator("*", 3, true) { }

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

class TLOForwardSlash : public IOperator
{
public:
	TLOForwardSlash() : IOperator("/", 3, true) { }

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

class TLODot : public IOperator
{
public:
	TLODot() : IOperator(".", 1, true) {}

	void parse(std::vector<Token>::iterator beforeStart, std::vector<Token>::iterator beforeEnd,
		std::vector<Token>::iterator afterStart, std::vector<Token>::iterator afterEnd, Script& script);
};

#endif