#include "json.hpp"

static void parse_value(fa_json& ret, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err);

static void await_character(const char c, bool eoi_on_end, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err)
{
	while (it != end && (*it == ' ' || *it == '\n' || *it == '\r' || *it == '\t'))
		it++;

	if (it == end && eoi_on_end)
	{
		err->code = fa_json_errno::end_of_input;
		err->description = "End of input when looking for '" + (c ? std::string(1, c) : "non-whitespace character") + "'.";
		err->it = it;
	}
	else if (c && *it != c)
	{
		err->code = fa_json_errno::unexpected_character;
		err->description = "Unexpected character '" + std::string(1, *it) + "' when looking for '" + std::string(1, c) + "'.";
		err->it = it;
	}
}

static void parse_string(std::string& ret, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err)
{
	bool ignore_next = false;

	while (it != end && (ignore_next || *it != '\"'))
	{
		if (!ignore_next && *it == '\\')
			ignore_next = true;
		else
		{
			ret.push_back(*it);
			ignore_next = false;
		}

		it++;
	}

	if (it == end)
	{
		err->code = fa_json_errno::end_of_input;
		err->description = "Unexpected end of input when parsing a string.";
		err->it = it;
	}
	else
		it++;
}

static void parse_object(std::map<std::string, fa_json>& ret, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err)
{
	std::string key;
	fa_json value;

	while (true)
	{
		// Wait for string
		await_character(0, true, it, end, err);

		if (*it == '"')
			parse_string(key, ++it, end, err);
		else if (*it == '}')
			break;
		else
		{
			err->code = fa_json_errno::unexpected_character;
			err->description = "Unexpected character '" + std::string(1, *it) + "' when parsing object.";
			err->it = it;
			return;
		}

		// Wait for :
		await_character(':', true, it, end, err);

		it++;

		parse_value(value, it, end, err);

		ret.insert({ key, value });

		await_character(0, true, it, end, err);

		if (*it == ',')
		{
			key.clear();
			value = fa_json::object();
			it++;
		}
		else if (*it == '}')
		{
			break;
		}
		else
		{
			err->code = fa_json_errno::unexpected_character;
			err->description = "Unexpected character '" + std::string(1, *it) + "' when parsing object.";
			err->it = it;
			return;
		}
	}

	it++;
}

static void parse_array(std::vector<fa_json>& ret, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err)
{
	fa_json value;

	while (true)
	{
		await_character(0, true, it, end, err);

		if (*it == ']')
			break;

		parse_value(value, it, end, err);

		ret.push_back(value);

		await_character(0, true, it, end, err);

		if (*it == ',')
		{
			value = fa_json::object();
			it++;
		}
		else if (*it == ']')
		{
			break;
		}
		else
		{
			err->code = fa_json_errno::unexpected_character;
			err->description = "Unexpected character '" + std::string(1, *it) + "' when parsing array.";
			err->it = it;
			return;
		}
	}

	it++;
}

// true -> floating point, false -> integer
static bool parse_number(long double& ret, std::string::const_iterator& it, std::string::const_iterator end)
{
	bool is_floating = false;
	long double mul = 1;
	ret = 0;

	if (*it == '-')
		mul = -1;

	while (it != end && *it >= '0' && *it <= '9')
	{
		ret *= 10;
		ret += *it - '0';
		it++;
	}

	if (it != end && *it == '.')
	{
		is_floating = true;
		long double divisor = 1;
		it++;

		while (it != end && *it >= '0' && *it <= '9')
		{
			divisor *= 10;
			ret += (*it - '0') / divisor;
			it++;
		}
	}

	ret *= mul;

	return is_floating;
}

static void parse_value(fa_json& ret, std::string::const_iterator& it, std::string::const_iterator end, fa_json_error* err)
{
	await_character(0, false, it, end, err);

	if (it != end)
	{
		switch (*it)
		{
		case '{':
			ret = fa_json::object();
			parse_object(std::get<fa_json::object>(ret), ++it, end, err);
			break;

		case '[':
			ret = fa_json::arr();
			parse_array(std::get<fa_json::arr>(ret), ++it, end, err);
			break;

		case '"':
			ret = fa_json::string();
			parse_string(std::get<fa_json::string>(ret), ++it, end, err);
			break;

		default:
			if ((*it >= '0' && *it <= '9') || *it == '.' || *it == '-')
			{
				long double val = 0;

				// This time we don't advance the iterator, as the first character is already a part of the value.
				if (parse_number(val, it, end))
				{
					ret = val;
				}
				else
				{
					ret = (long long)std::floor(val);
				}
			}
			else
			{
				err->code = fa_json_errno::unexpected_character;
				err->description = "Unexpected character " + std::string(1, *it) + ".";
				err->it = it;
			}
		}
	}
	else
	{
		ret = fa_json::object();
	}
}

void fa_json::parse(const std::string& code, fa_json_error* err)
{
	auto it = code.begin();
	auto end = code.end();

	fa_json_error error;

	if (it != end)
		parse_value(*this, it, end, &error);
	else
	{
		*this = fa_json::object();
	}

	if (err)
		*err = error;
}

static void dump_value(std::string& ret, const fa_json& value);

static void dump_array(std::string& ret, const fa_json::arr& value)
{
	ret.push_back('[');

	for (size_t i = 0; i < value.size(); i++)
	{
		dump_value(ret, value[i]);
		if (i < value.size() - 1)
			ret.push_back(',');
	}

	ret.push_back(']');
}

static void dump_floating(std::string& ret, fa_json::floating value)
{
	ret += std::to_string(value);
}

static void dump_integer(std::string& ret, fa_json::integer value)
{
	ret += std::to_string(value);
}

static void dump_string(std::string& ret, const fa_json::string& value)
{
	ret.push_back('"');

	ret += value;

	ret.push_back('"');
}

static void dump_object(std::string& ret, const fa_json::object& value)
{
	ret.push_back('{');

	size_t i = 0;

	for (const auto& [key, val] : value)
	{
		dump_string(ret, key);

		ret.push_back(':');

		dump_value(ret, val);

		if (i < value.size() - 1)
			ret.push_back(',');

		i++;
	}

	ret.push_back('}');
}

static void dump_value(std::string& ret, const fa_json& value)
{
	switch (value.index())
	{
	case FA_JSON_ARRAY:
		dump_array(ret, std::get<fa_json::arr>(value));
		break;

	case FA_JSON_FLOATING:
		dump_floating(ret, std::get<fa_json::floating>(value));
		break;

	case FA_JSON_INTEGER:
		dump_integer(ret, std::get<fa_json::integer>(value));
		break;

	case FA_JSON_OBJECT:
		dump_object(ret, std::get<fa_json::object>(value));
		break;

	case FA_JSON_STRING:
		dump_string(ret, std::get<fa_json::string>(value));
		break;
	}
}

std::string fa_json::dump() const
{
	std::string ret;

	dump_value(ret, *this);

	return ret;
}

std::istream& operator>>(std::istream& input, fa_json& output)
{
	std::string line;
	std::string source;

	while (std::getline(input, line))
		source += line;

	fa_json_error err;

	output.parse(source, &err);

	if (err.code != fa_json_errno::ok)
	{
		throw std::runtime_error("Invalid JSON file. Could not parse. (operator>>).");
	}

	return input;
}

std::ostream& operator<<(std::ostream& output, const fa_json& input)
{
	return output << input.dump();
}

fa_json_error::fa_json_error(fa_json_errno err, const std::string& desc, std::string::const_iterator iterator)
	: code(err), description(desc), it(iterator)
{
}
