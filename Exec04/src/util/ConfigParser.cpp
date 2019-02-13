/*
 * ConfigParser.cpp
 *
 *  Created on: 2019. 2. 12.
 *      Author: fineg
 */


#include "ConfigParser.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


namespace stringUtil
{
	std::vector<std::string> split(const std::string& string, char delimiters = ' ')
	{
		std::vector<std::string> result;
		std::stringstream stream(string);
		std::string word;
		while(std::getline(stream, word, delimiters))
		{
			result.push_back(word);
		}
		return result;
	}
}

namespace
{
	bool isComment(const std::string& line)
	{
		return line[0] == ';';
	}

	std::string extractSectionName(const std::string& line)
	{
		return std::string(line.begin()+1, line.end()-1);
	}

	bool isSectionHeading(const std::string& line)
	{
		if(line[0] != '[' || line[line.size()-1] != ']')
		{
			return false;
		}
		const std::string sectionName = extractSectionName(line);
		return std::all_of(sectionName.cbegin(), sectionName.cend(), [](char c) { return std::isalpha(c); });
	}

	bool isKeyValuePair(const std::string& line)
	{
		return std::count(line.begin(), line.end(), '=') == 1;
	}

	void ensureSectionIsUnique(const std::string& section_name, const Config& sections)
	{
		if(sections.count(section_name) != 0)
		{
			throw std::runtime_error(section_name + " appears twice in config file");
		}
	}

	void ensureKeyIsUnique(const std::string& key, const Section& section)
	{
		if(section.count(key) != 0)
		{
			throw std::runtime_error(key + " appears twice in section");
		}
	}

	void ensureCurrentSection(const std::string& line, const std::string& current_section)
	{
		if(current_section.empty())
		{
			throw std::runtime_error(line + " does not occur within a section");
		}
	}

    std::pair<std::string, std::string> parseKeyValuePair(const std::string& line)
    {
        std::vector<std::string> pair = stringUtil::split(line, '=');
        return std::pair <std::string, std::string>(pair[0], pair[1]);
    }
}

ConfigParser::ConfigParser(const std::string& path_name) : mCurrentSection(""), mSections{parseFile(path_name)} {}

std::vector<std::string> ConfigParser::getSections() const
{
	std::vector<std::string> sectionNames;
	for (auto it = mSections.begin();it != mSections.end();++it) {
//		sectionNames.push_back(it->first);
	}
	return sectionNames;
}

Config ConfigParser::parseFile(const std::string& path_name)
{
	Config sections;
	std::ifstream input(path_name);
	std::string line;
	while(std::getline(input, line))
	{
		parseLine(line, sections);
	}
	return sections;
}

void ConfigParser::parseLine(const std::string& line, Config& sections)
{
    if (isComment(line))
    {
        return;
    }
    else if (isSectionHeading(line))
    {
        addSection(line, sections);
    }
    else if (isKeyValuePair(line))
    {
        addKeyValuePair(line, sections);
    }
}

void ConfigParser::addSection(const std::string& line, Config& sections)
{
    const std::string sectionName = extractSectionName(line);
    ensureSectionIsUnique(sectionName, sections);
    Section s{{sectionName, ""}};
    sections.emplace(sectionName, s);
    mCurrentSection = sectionName;
}

void ConfigParser::addKeyValuePair(const std::string& line, Config& sections) const
{
    ensureCurrentSection(line, mCurrentSection);
    const auto keyValuePair = parseKeyValuePair(line);
    ensureKeyIsUnique(keyValuePair.first, sections.at(mCurrentSection));
    sections.at(mCurrentSection).emplace(keyValuePair);
//    sections.at(mCurrentSection).insert(keyValuePair);
}


void a01() {

	Section s{{"a", "b"}};

	Config c{{"aa", s}};

	c["cccc"] = {{"1", "2"}};
}

