#ifndef LANGUAGE_FUNCTIONS_H
#define LANGUAGE_FUNCTIONS_H

#include "ilanguage_function.h"
#include <iostream>
#include <Windows.h>

class LFEcho : public ILanguageFunction
{
public:
	LFEcho() { }
	void call(std::vector<std::string> args)
	{
		if(args.size() == 1) {
			std::cout << args[0];
		}
	}
};
class LFEchoLine : public ILanguageFunction
{
public:
	LFEchoLine() {}
	void call(std::vector<std::string> args)
	{
		if(args.size() == 1) {
			std::cout << args[0] << std::endl;
		} else {
			std::cout << std::endl;
		}
	}
};
class LFMsgbox : public ILanguageFunction
{
public:
	LFMsgbox() {}
	void call(std::vector<std::string> args)
	{
		if(args.size() == 2) {
			MessageBoxA(NULL, args[0].c_str(), args[1].c_str(), MB_OK);
		} else if(args.size() == 1) {
			MessageBoxA(NULL, args[0].c_str(), "", MB_OK);
		} else {
			throw LanguageFunctionException("msgbox() takes 1 or 2 arguments");
		}
	}
};

#endif