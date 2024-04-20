#include "StringTools.h"
#include <vector>
#include <string>

namespace Tools
{
	std::vector<std::string> SplitString(const char* input, const char* delimiter)
	{
		std::string mystring(input);
		return SplitString(mystring, delimiter);
	}


	std::vector<std::string> SplitString(const std::string& input, const char* delimiter)
	{
		std::vector<std::string> tokens;
		std::string token;
		std::string inputCopy = input; // Create a copy of the input string
		size_t pos = 0;
		while ((pos = inputCopy.find(delimiter)) != std::string::npos)
		{
			token = inputCopy.substr(0, pos);
			tokens.push_back(token);
			inputCopy.erase(0, pos + 1);
		}
		tokens.push_back(inputCopy); // Add the last token
		return tokens;
	}

	std::string Trim(const char* str)
	{
		std::string mystring(str);
		return Trim(mystring);
	}

	std::string Trim(const std::string& str)
	{
		std::size_t start = str.find_first_not_of(" \t\n\r");
		std::size_t end = str.find_last_not_of(" \t\n\r");

		if (start == std::string::npos || end == std::string::npos)
		{
			// The string contains only whitespace characters or is empty
			return "";
		}

		return str.substr(start, end - start + 1);
	}


	std::string RemovePrefix(const std::string& str, const std::string& prefix)
	{
		if (str.substr(0, prefix.size()) == prefix)
			return str.substr(prefix.size());
		else
			return str;
	}


	std::string RemoveSuffix(const std::string& str, const std::string& suffix)
	{
		if (str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix)
			return str.substr(0, str.size() - suffix.size());
		else
			return str; // Suffix not found, return original string
	}

	std::string Replace(std::string str, const std::string& toBeReplaced, const std::string& replacement)
	{
		size_t pos = 0;
		while ((pos = str.find(toBeReplaced, pos)) != std::string::npos)
		{
			str.replace(pos, toBeReplaced.length(), replacement);
			pos += replacement.length(); // Advance to the end of the replacement
		}
		return str;
	}




}