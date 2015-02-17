#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "language/ilanguage.h"
#include "language/language_operators.h"

class Language : public ILanguage
{
public:
	Language();
	
	SITokenType tokenType(const std::string& tokenStr);
	void registerFunctions(CPU& c);

private:
	TLOEquals m_operatorEquals;
	TLOPlus m_operatorPlus;
	TLOHyphen m_operatorHyphen;
	TLOAsterisk m_operatorAsterisk;
	TLOForwardSlash m_operatorForwardSlash;
};

#endif