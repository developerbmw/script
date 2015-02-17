#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <sstream>

class TypeException
{
public:
	TypeException(std::string msg) : m_msg(msg) {}

	std::string getErrorMessage() { return m_msg; }

private:
	std::string m_msg;
};

class Value
{
public:
	Value(std::string value) : m_value(value) { }
	Value() : m_value("") { }

	std::string asString()
	{
		return m_value;
	}

	int asInt()
	{
		std::istringstream ss(m_value);
		int val;
		ss >> val;
		if(ss.fail() || !ss.eof()) {
			throw TypeException("Cannot convert value \"" + m_value + "\" to int");
		}
		return val;
	}

	long long asLong()
	{
		std::istringstream ss(m_value);
		long long val;
		ss >> val;
		if(ss.fail() || !ss.eof()) {
			throw TypeException("Cannot convert value \"" + m_value + "\" to long");
		}
		return val;
	}

	double asDouble()
	{
		std::istringstream ss(m_value);
		double val;
		ss >> val;
		if(ss.fail() || !ss.eof()) {
			throw TypeException("Cannot convert value \"" + m_value + "\" to double");
		}
		return val;
	}

private:
	std::string m_value;
};

#endif