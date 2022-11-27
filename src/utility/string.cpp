#include "utility/string.h"

#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <cctype>

namespace aoc
{
	std::vector<std::string> split(const std::string& str, const char delim)
	{
		std::vector<std::string> split_strings;
		std::istringstream iss(str);
		for (std::string temp; std::getline(iss, temp, delim); )
		{
			split_strings.push_back(temp);
		}

		return split_strings;
	}

	std::vector<std::string> regex_split(const std::string& str, const std::string& pattern)
	{
		std::regex re(pattern);
		return std::vector<std::string>(
			std::sregex_token_iterator(str.begin(), str.end(), re, -1),
			std::sregex_token_iterator());
	}

	bool is_lower(const std::string& str)
	{
		return std::all_of(str.begin(), str.end(), [](const char c) { return std::islower(c); });
	}

	bool is_upper(const std::string& str)
	{
		return std::all_of(str.begin(), str.end(), [](const char c) { return std::isupper(c); });
	}

} // aoc
