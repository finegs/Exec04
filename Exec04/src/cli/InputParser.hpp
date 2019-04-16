/*
 * InputParser.hpp
 *
 *  Created on: 2018. 12. 8.
 *      Author: finegs
 */

#ifndef SRC_UDP_INPUTPARSER_HPP_
#define SRC_UDP_INPUTPARSER_HPP_

#include <string>
#include <vector>


class InputParser {
public:
	InputParser(int &argc,  char ** argv);
	const std::string& getCmdOption(const std::string& option) const;
	bool cmdOptionExists(const std::string& option) const ;

private:
	std::vector<std::string> tokens;
};


#endif /* SRC_UDP_INPUTPARSER_HPP_ */
