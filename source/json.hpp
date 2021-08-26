#pragma once
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <variant>
#include <exception>

#define FA_JSON_INTEGER 0
#define FA_JSON_FLOATING 1
#define FA_JSON_STRING 2
#define FA_JSON_OBJECT 3
#define FA_JSON_ARRAY 4

enum class fa_json_errno
{
	ok,
	unexpected_character,
	end_of_input
};

struct fa_json_error
{
	fa_json_errno code = fa_json_errno::ok;
	std::string description;
};

class fa_json;

class fa_json : public std::variant<long long, long double, std::string, std::map<std::string, fa_json>, std::vector<fa_json>>
{
public:
	using base = std::variant<long long, long double, std::string, std::map<std::string, fa_json>, std::vector<fa_json>>;
	using base::base;

	using integer = int64_t;
	using floating = long double;
	using string = std::string;
	using object = std::map<std::string, fa_json>;
	using arr = std::vector<fa_json>;

	fa_json_error load(std::istream& code);
	fa_json_error loads(const std::string& code);
	void dump(std::ostream& output) const;
	std::string dumps() const;
};

std::istream& operator>>(std::istream& input, fa_json& output);
std::ostream& operator<<(std::ostream& output, const fa_json& input);