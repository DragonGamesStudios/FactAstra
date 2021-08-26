#include "json.hpp"

#include <sstream>

static fa_json_error load_value(fa_json& ret, std::istream& input);

static fa_json_error await_character(const char c, bool eoi_on_end, std::istream& input)
{
	fa_json_error error;

	char next = input.peek();
	char _;

	while (!input.eof() && (next == ' ' || next == '\n' || next == '\r' || next == '\t'))
	{
		input.get();
		next = input.peek();
	}

	if (input.eof() && eoi_on_end)
	{
		error.code = fa_json_errno::end_of_input;
		error.description = "End of input when looking for '" + (c ? std::string(1, c) : "non-whitespace character") + "'.";
	}
	else if (c && next != c)
	{
		error.code = fa_json_errno::unexpected_character;
		error.description = "Unexpected character '" + std::string(1, next) + "' when looking for '" + std::string(1, c) + "'.";
	}

	return error;
}

static fa_json_error load_string(std::string& ret, std::istream& input)
{
	fa_json_error error;
	bool ignore_next = false;
	char c = input.get();

	while (!input.eof() && (ignore_next || c != '\"'))
	{
		if (!ignore_next && c == '\\')
			ignore_next = true;
		else
		{
			ret.push_back(c);
			ignore_next = false;
		}

		c = input.get();
	}

	if (input.eof())
	{
		error.code = fa_json_errno::end_of_input;
		error.description = "Unexpected end of input when parsing a string.";
	}

	return error;
}

static fa_json_error load_object(std::map<std::string, fa_json>& ret, std::istream& input)
{
	std::string key;
	fa_json value;
	fa_json_error error;
	char c = 0;

	while (true)
	{
		// Wait for string
		error = await_character(0, true, input);

		if (error.code != fa_json_errno::ok)
			return error;

		c = input.get();

		if (c == '"')
			error = load_string(key, input);
		else if (c == '}')
			break;
		else
		{
			error.code = fa_json_errno::unexpected_character;
			error.description = "Unexpected character '" + std::string(1, c) + "' when parsing object.";
			return error;
		}

		if (error.code != fa_json_errno::ok)
			return error;

		// Wait for :
		await_character(':', true, input);

		input.get();

		error = load_value(value, input);

		if (error.code != fa_json_errno::ok)
			return error;

		ret.insert({ key, value });

		error = await_character(0, true, input);

		if (error.code != fa_json_errno::ok)
			return error;

		c = input.get();

		if (c == ',')
		{
			key.clear();
			value = fa_json::object();
		}
		else if (c == '}')
		{
			break;
		}
		else
		{
			error.code = fa_json_errno::unexpected_character;
			error.description = "Unexpected character '" + std::string(1, c) + "' when parsing object.";
			return error;
		}
	}

	return error;
}

static fa_json_error load_array(std::vector<fa_json>& ret, std::istream& input)
{
	fa_json_error error;
	fa_json value;
	char c = 0;

	while (true)
	{
		// Can fail
		error = await_character(0, true, input);

		if (error.code != fa_json_errno::ok)
			return error;

		if (input.peek() == ']')
		{
			input.get();
			break;
		}

		error = load_value(value, input);

		if (error.code != fa_json_errno::ok)
			return error;

		ret.push_back(value);

		error = await_character(0, true, input);

		if (error.code != fa_json_errno::ok)
			return error;

		c = input.get();

		if (c == ',')
			value = fa_json::object();
		else if (c == ']')
			break;
		else
		{
			error.code = fa_json_errno::unexpected_character;
			error.description = "Unexpected character '" + std::string(1, c) + "' when parsing array.";
			return error;
		}
	}

	return error;
}

// true -> floating point, false -> integer
static bool load_number(long double& ret, std::istream& input)
{
	bool is_floating = false;
	long double mul = 1;
	char c = input.get();
	ret = 0;

	if (c == '-')
		mul = -1;

	while (!input.eof() && c >= '0' && c <= '9')
	{
		ret *= 10;
		ret += c - '0';
		c = input.get();
	}

	if (!input.eof() && c == '.')
	{
		is_floating = true;
		long double divisor = 1;
		c = input.get();

		while (!input.eof() && c >= '0' && c <= '9')
		{
			divisor *= 10;
			ret += (c - '0') / divisor;
			c = input.get();
		}
	}

	ret *= mul;
	input.unget();

	return is_floating;
}

static fa_json_error load_value(fa_json& ret, std::istream& input)
{
	// Cannot fail
	fa_json_error error = await_character(0, false, input);

	if (!input.eof())
	{
		char c = input.get();

		switch (c)
		{
		case '{':
			ret = fa_json::object();
			error = load_object(std::get<fa_json::object>(ret), input);
			break;

		case '[':
			ret = fa_json::arr();
			error = load_array(std::get<fa_json::arr>(ret), input);
			break;

		case '"':
			ret = fa_json::string();
			error = load_string(std::get<fa_json::string>(ret), input);
			break;

		default:
			if ((c >= '0' && c <= '9') || c == '.' || c == '-')
			{
				long double val = 0;

				// This time we  unget the character, as the loaded character is already a part of the value.
				input.unget();

				// Doesn't return an error
				if (load_number(val, input))
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
				error.code = fa_json_errno::unexpected_character;
				error.description = "Unexpected character " + std::string(1, c) + ".";
			}
		}
	}
	else
	{
		ret = fa_json::object();
	}

	return error;
}

fa_json_error fa_json::load(std::istream& input)
{
	fa_json_error error;

	if (!input.eof())
		error = load_value(*this, input);
	else
	{
		*this = fa_json::object();
	}

	return error;
}

fa_json_error fa_json::loads(const std::string& code)
{
	std::stringstream ss;

	ss << code;

	return load(ss);
}

static void dump_value(std::ostream& ret, const fa_json& value);

static void dump_array(std::ostream& ret, const fa_json::arr& value)
{
	ret << '[';

	for (size_t i = 0; i < value.size(); i++)
	{
		dump_value(ret, value[i]);
		if (i < value.size() - 1)
			ret << ',';
	}

	ret << ']';
}

static void dump_floating(std::ostream& ret, fa_json::floating value)
{
	ret << value;
}

static void dump_integer(std::ostream& ret, fa_json::integer value)
{
	ret << value;
}

static void dump_string(std::ostream& ret, const fa_json::string& value)
{
	ret << '"' << value << '"';
}

static void dump_object(std::ostream& ret, const fa_json::object& value)
{
	ret << '{';

	size_t i = 0;

	for (const auto& [key, val] : value)
	{
		dump_string(ret, key);

		ret << ':';

		dump_value(ret, val);

		if (i < value.size() - 1)
			ret << ',';

		i++;
	}

	ret << '}';
}

static void dump_value(std::ostream& ret, const fa_json& value)
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

void fa_json::dump(std::ostream& output) const
{
	dump_value(output, *this);
}

std::string fa_json::dumps() const
{
	std::stringstream ret;

	dump_value(ret, *this);

	return ret.str();
}

std::istream& operator>>(std::istream& input, fa_json& output)
{
	fa_json_error err = output.load(input);

	if (err.code != fa_json_errno::ok)
	{
		throw std::runtime_error("Invalid JSON file. Could not parse. (operator>>).");
	}

	return input;
}

std::ostream& operator<<(std::ostream& output, const fa_json& input)
{
	input.dump(output);
	return output;
}
