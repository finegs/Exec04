/*
 * AppParser.cpp
 *
 *  Created on: 2018. 12. 8.
 *      Author: finegs
 */
#include <iostream>
#include <cstdlib>
#include "InputParser.hpp"

int main(int argc, char* argv[]) {

	InputParser ps  = InputParser(argc, argv);
	if(ps.cmdOptionExists("-h")) {
		std::cout << ps.getCmdOption("-h").c_str() << std::endl;
	}
	else if(ps.cmdOptionExists("-d")) {

	}

	return EXIT_SUCCESS;
}



