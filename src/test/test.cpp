#include <iostream>
#include "program.h"
#include <fstream>

int main()
{
	std::ostream& output = std::cout;

	try {
		Program program(std::ifstream("src\\test\\test_script.txt"));
		program.run(output);
	} catch(ParserException& e) {
		output << "Error - " << e.getErrorMessage() << std::endl;
	}

	return 0;
}