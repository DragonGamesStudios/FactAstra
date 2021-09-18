#pragma once
#include <string>

enum class fa_errno
{
	ok,
	not_a_mod,
	invalid_json,
	higher_version_present,
	fs_entry_does_not_exist,
	already_loaded,
	invalid_filename,
	mod_incompatible,
	mod_ignored,
	invalid_version_string,
	lua_invalid_prototype,
	lua_error
};

struct fa_Error
{
	fa_errno code = fa_errno::ok;
	std::string description;
	void* user_pointer = 0;
};