/*
 * mutil.cpp
 *
 *  Created on: 2019. 2. 12.
 *      Author: fineg
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include "mutil.hpp"

using namespace std;

int readIni(const char* file_name, const std::unordered_map<std::string, std::string> &map)
{
	int rc;
	string line;
	if(nullptr == file_name)
	{
		cerr << __FILE__ << __LINE__ << "file_name is empty";
		return EXIT_FAILURE;
	}
	ifstream file(file_name);
	if(!file.is_open())
	{
		cerr << __FILE__ << __LINE__ << "file is not exist. file=" << file_name;
		return EXIT_FAILURE;
	}

	while(getline(file, line))
	{

	}

	file.close();

	rc = EXIT_SUCCESS;

	return rc;
}

