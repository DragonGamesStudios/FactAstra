#include "util.hpp"

namespace fa_util
{
	std::vector<std::string> split(const std::string& input, const std::string& delim)
	{
		std::vector<std::string> ret;
		size_t start = 0;
		size_t end = input.find(delim);

		while (end != std::string::npos)
		{
			ret.push_back(input.substr(start, end - start));
			start = end + delim.length();
			end = input.find(delim, start);
		}


		ret.push_back(input.substr(start, end));

		return ret;
	}
}