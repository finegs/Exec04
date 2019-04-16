/*
 * InputParser.cpp
 *
 *  Created on: 2018. 12. 8.
 *      Author: finegs
 */
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

#include "InputParser.hpp"

InputParser::InputParser(int &argc,  char ** argv) {
	for(int i=1;i<argc;i++) {
		this->tokens.push_back(std::string(argv[i]));
	}
}

const std::string& InputParser::getCmdOption(const std::string& option) const {
	std::vector<std::string>::const_iterator itr;
	itr = std::find(this->tokens.begin(), this->tokens.end(), option.c_str());
	if(itr != this->tokens.end() && ++itr != this->tokens.end())
		return *itr;

	static const std::string empty_string("");
	return empty_string;
}

bool InputParser::cmdOptionExists(const std::string& option) const {
	return std::find(this->tokens.begin(), this->tokens.end(), option.c_str()) != this->tokens.end();
}

