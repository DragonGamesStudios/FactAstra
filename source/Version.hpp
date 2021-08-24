#pragma once
#include <string>
#include <memory>

#define FACTASTRA_VERSION_MAJOR 0
#define FACTASTRA_VERSION_MINOR 0
#define FACTASTRA_VERSION_PATCH 0

struct fa_Version
{
	uint32_t major = 0;
	uint32_t minor = 0;
	uint32_t patch = 0;

	bool major_parsed = false;
	bool minor_parsed = false;
	bool patch_parsed = false;

	fa_Version(uint32_t maj = 0, uint32_t min = 0, uint32_t pat = 0);
	
	char parse(const std::string& vstr);
	std::string dump() const;

	bool operator==(const fa_Version& other) const;
	bool operator!=(const fa_Version& other) const;
	bool operator>(const fa_Version& other) const;
	bool operator>=(const fa_Version& other) const;
};

extern fa_Version factastra_version;