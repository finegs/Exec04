/*
 * ConfigParser.hpp
 *
 *  Created on: 2019. 2. 12.
 *      Author: fineg
 */

#ifndef CONFIGPARSER_HPP_
#define CONFIGPARSER_HPP_

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

int readIni(const char* file_name, const std::unordered_map<std::string, std::string> &map);

//typedef std::map<std::string, std::string> Section;
//typedef std::map<std::string, Section> Config;
using Section = std::map<std::string, std::string>;
using Config = std::map<std::string, Section>;

class ConfigParser
{
public:
	ConfigParser(const std::string& path_name);
	Section getSection(const std::string& section_name) const
	{
		return mSections.at(section_name);
	}

	std::string get(const std::string& section_name, const std::string& key) const
	{
		return mSections.at(section_name).at(key);
	}
private:
	std::vector<std::string> getSections() const;

private:
	Config parseFile(const std::string& path_name);
	void parseLine(const std::string& line, Config& sections);
	void addSection(const std::string& line, Config& sections);
	void addKeyValuePair(const std::string& line, Config& sections) const;

	std::string mCurrentSection;
	const Config mSections;
};


#endif /* CONFIGPARSER_HPP_ */
