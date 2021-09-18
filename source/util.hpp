#pragma once
#include "errors.hpp"

#include <Spectre2D/FileSystem.h>
#include <lua/lua.hpp>

#include <vector>
#include <string>
#include <functional>
#include <set>

namespace fa_util
{
	std::vector<std::string> split(const std::string& input, const std::string& delim);

	// Lua utilities
	typedef std::function<int(lua_State*)> lua_function;

	void lua_push_function(lua_State* L, const lua_function& fn);

	bool lua_move_value(lua_State* from, lua_State* to, int from_idx, std::set<const void*>* cyclic_data_memory_ptr);

	void lua_prepare_default_state(lua_State* L, sp::FileSystem* fs);

	void lua_retrieve_error_message(lua_State*L, int index, std::string* msg, std::string* traceback);

	int lua_generic_error_handler(lua_State* L);

	// Lua constraints

	bool lua_constraint_has_field(lua_State* L, int index, const std::string& field, bool pop = true);

	bool lua_constraint_has_field_of_type(lua_State* L, int index, const std::string& field, int type, bool pop = true);

	bool lua_constraint_string_equal(lua_State* L, int index, const std::string& string);

	bool lua_constraint_is_of_type(lua_State* L, int index, int type);
}