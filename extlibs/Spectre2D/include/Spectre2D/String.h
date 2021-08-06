#pragma once
#include <memory>
#include <string>
#include <iostream>

namespace sp
{
	class StringUTF8 : public std::string
	{
	public:
		StringUTF8();
		StringUTF8(const char* _Ptr);
		StringUTF8(const std::string& s);
	};
}