#include "Version.hpp"
#include "errors.hpp"

static int64_t parse_integer(std::string::const_iterator& it, std::string::const_iterator end)
{
	int64_t ret = 0;

	while (it != end && *it >= '0' && *it <= '9')
	{
		ret *= 10;
		ret += (int64_t)*it - '0';
		it++;
	}

	return ret;
}

fa_Version::fa_Version(uint32_t maj, uint32_t min, uint32_t pat)
	: major(maj), minor(min), patch(pat)
{
}

char fa_Version::parse(const std::string& vstr)
{
	major = minor = patch = 0;

	auto it = vstr.begin();

	major = parse_integer(it, vstr.end());
	major_parsed = true;

	if (it == vstr.end())
		return 0;

	if (*it != '.')
		return *it;

	it++;

	if (it == vstr.end())
		return '.';

	minor = parse_integer(it, vstr.end());
	minor_parsed = true;

	if (it == vstr.end())
		return 0;

	if (*it != '.')
		return *it;

	it++;

	if (it == vstr.end())
		return '.';

	patch = parse_integer(it, vstr.end());
	patch_parsed = true;

	if (it != vstr.end())
		return *it;

	return 0;
}

std::string fa_Version::dump() const
{
	return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
}

bool fa_Version::operator==(const fa_Version& other) const
{
	return major == other.major && minor == other.minor && patch == other.patch;
}

bool fa_Version::operator!=(const fa_Version& other) const
{
	return major != other.major || minor != other.minor || patch != other.patch;
}

bool fa_Version::operator>(const fa_Version& other) const
{
	return major > other.major || (major == other.major && (minor > other.minor || (minor == other.minor && (patch > other.patch))));
}

bool fa_Version::operator>=(const fa_Version& other) const
{
	return major > other.major || (major == other.major && (minor > other.minor || (minor == other.minor && (patch >= other.patch))));
}

fa_Version factastra_version = fa_Version(FACTASTRA_VERSION_MAJOR, FACTASTRA_VERSION_MINOR, FACTASTRA_VERSION_PATCH);