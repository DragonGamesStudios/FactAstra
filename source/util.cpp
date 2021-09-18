#include "util.hpp"

#include <sstream>
#include <set>

static const char lua_function_mt[] = "FACTASTRA_FUNCTION";

static int lua_fn_on_call(lua_State* L)
{
	fa_util::lua_function* f = static_cast<fa_util::lua_function*>(luaL_checkudata(L, 1, lua_function_mt));
	lua_remove(L, 1);
	return (*f)(L);
}

static int lua_fn_on_garbage_collection(lua_State* L)
{
	fa_util::lua_function* f = static_cast<fa_util::lua_function*>(luaL_checkudata(L, 1, lua_function_mt));
	if (!f)
		luaL_error(L, "C++ error: problem with function deletion");
	else
	{
		//delete f;
		f->~function();
	}

	return 0;
}

static int lua_fn_on_tostring(lua_State* L)
{
	fa_util::lua_function* f = static_cast<fa_util::lua_function*>(luaL_checkudata(L, 1, lua_function_mt));

	std::stringstream ss;
	ss << "C++ function on " << std::hex << f;
	lua_pushstring(L, ss.str().c_str());

	return 1;
}

// Lua function storage
static const char* lf_reader(lua_State* L, void* ud, size_t* size)
{
	std::string* lf = (std::string*)ud;

	*size = lf->size();
	return lf->size() ? lf->c_str() : 0;
}

static int lf_writer(lua_State* L, const void* b, size_t size, void* ud)
{
	std::string* lf = (std::string*)ud;

	lf->append(std::string((const char*)b, size));

	return 0;
}

// Customized require
static std::filesystem::path lua_correct_path(const std::string& lua_path)
{
	std::string path = lua_path;

	for (int i = 0; i < lua_path.size(); i++)
	{
		if (lua_path[i] == '.')
			path[i] = '/';
	}

	path += ".lua";

	std::filesystem::path ret = path;

	return ret;
}

static int lua_require(lua_State* L, sp::FileSystem* fs)
{
	if (!lua_isstring(L, 1))
		luaL_error(L, "Invalid argument to 'require'. String expected, got %s.", lua_typename(L, lua_type(L, 1)));

	std::filesystem::path executed_file = lua_correct_path(lua_tostring(L, 1));

	if (!fs->exists(executed_file))
		luaL_error(L, "File on path %s not found.", executed_file.string().c_str());

	int beginning_stack_size = lua_gettop(L);

	// Execute the file
	if (luaL_dofile(L, fs->getCorrectPath(executed_file).string().c_str()) != LUA_OK)
		luaL_error(L, lua_tostring(L, -1));

	int end_stack_size = lua_gettop(L);

	return end_stack_size - beginning_stack_size;
}

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

	void lua_push_function(lua_State* L, const lua_function& fn)
	{
		// Allocate memory
		void* new_fn = lua_newuserdata(L, sizeof(lua_function));

		// Set metatable to func
		luaL_getmetatable(L, lua_function_mt);
		lua_setmetatable(L, -2);

		// Set variable
		new (new_fn) lua_function(fn);
	}

	bool lua_move_value(lua_State* from, lua_State* to, int from_idx, std::set<const void*>* cyclic_data_memory_ptr)
	{
		lua_pushvalue(from, from_idx);
		auto* cyclic_data_memory = cyclic_data_memory_ptr;

		if (!cyclic_data_memory_ptr)
		{
			cyclic_data_memory = new std::set<const void*>;
		}

		std::set<const void*>::iterator table_ptr;
		const void* ptr = 0;
		std::string f;

		switch (lua_type(from, -1))
		{
		case LUA_TNIL:
			lua_pushnil(to);
			break;

		case LUA_TNUMBER:
			lua_pushnumber(to, lua_tonumber(from, -1));
			break;

		case LUA_TBOOLEAN:
			lua_pushboolean(to, lua_toboolean(from, -1));
			break;

		case LUA_TFUNCTION:
			if (lua_dump(from, lf_writer, &f, false) != 0)
				luaL_error(from, "Something went wrong, when copying lua function.");

			lua_Debug ar;
			lua_getinfo(from, "n", &ar);

			if (lua_load(to, lf_reader, &f, ((std::string)"Function: " + ar.name).c_str(), 0) != LUA_OK)
				luaL_error(from, "Something went wrong, when copying lua function.");

			break;

		case LUA_TLIGHTUSERDATA:
			lua_pushlightuserdata(to, lua_touserdata(from, -1));
			break;

		case LUA_TSTRING:
			lua_pushstring(to, lua_tostring(from, -1));
			break;

		case LUA_TTABLE:
			ptr = lua_topointer(from, -1);
			table_ptr = cyclic_data_memory->find(ptr);

			if (table_ptr == cyclic_data_memory->end())
			{
				cyclic_data_memory->insert(ptr);

				lua_newtable(to);

				lua_pushnil(from);
				while (lua_next(from, -2))
				{
					lua_move_value(from, to, -2, cyclic_data_memory);
					lua_move_value(from, to, -1, cyclic_data_memory);

					if (lua_isnil(to, -2))
					{
						return false;
					}

					lua_settable(to, -3);

					lua_pop(from, 1);
				}

				cyclic_data_memory->erase(ptr);
			}
			else
			{
				luaL_error(from, "Recursive data detected");
			}

			break;

		default:
			lua_pushnil(to);
			return false;
			break;
		}

		lua_pop(from, 1);

		if (!cyclic_data_memory_ptr)
			delete cyclic_data_memory;

		return true;
	}

	void lua_prepare_default_state(lua_State* L, sp::FileSystem* fs)
	{
		// Load libraries
		luaL_requiref(L, "_G", luaopen_base, 1);
		luaL_requiref(L, "math", luaopen_math, 1);
		luaL_requiref(L, "string", luaopen_string, 1);
		luaL_requiref(L, "table", luaopen_table, 1);
		luaL_requiref(L, "debug", luaopen_debug, 1);

		lua_settop(L, 0);

		// Cpp function metatable
		luaL_newmetatable(L, lua_function_mt);

		// __call field (func())
		lua_pushcfunction(L, lua_fn_on_call);
		lua_setfield(L, -2, "__call");

		// __tostring field (tostring(func))
		lua_pushcfunction(L, lua_fn_on_tostring);
		lua_setfield(L, -2, "__tostring");

		// __gc field (garbage collection)
		lua_pushcfunction(L, lua_fn_on_garbage_collection);
		lua_setfield(L, -2, "__gc");

		// Pop
		lua_pop(L, 1);

		// Disable unsafe functions
		lua_getglobal(L, "_G");

		lua_pushnil(L);
		lua_setfield(L, -2, "dofile");

		lua_pushnil(L);
		lua_setfield(L, -2, "loadfile");

		lua_pop(L, 1);

		// Load require function
		lua_push_function(L, std::bind(lua_require, std::placeholders::_1, fs));
		lua_setglobal(L, "require");
	}

	void lua_retrieve_error_message(lua_State* L, int index, std::string* msg, std::string* traceback)
	{
		lua_getfield(L, index, "message");
		*msg = lua_tostring(L, -1);
		lua_pop(L, 1);

		lua_getfield(L, index, "traceback");
		*traceback = lua_tostring(L, -1);
		lua_pop(L, 1);
	}

	int lua_generic_error_handler(lua_State* L)
	{
		// Ensure stack size
		lua_settop(L, 1);

		// Create new error table
		lua_newtable(L);

		// Get the message
		lua_pushvalue(L, -2);
		lua_setfield(L, -2, "message");

		// Get the traceback
		luaL_traceback(L, L, 0, 0);
		lua_setfield(L, -2, "traceback");

		return 1;
	}

	bool lua_constraint_has_field(lua_State* L, int index, const std::string& field, bool pop)
	{
		bool ret = true;

		// Push key and get field
		lua_pushstring(L, field.c_str());
		if (lua_rawget(L, index < 0 ? (index - 1) : index) == LUA_TNIL)
			ret = false;

		if (pop)
			lua_pop(L, 1);

		return ret;
	}

	bool lua_constraint_has_field_of_type(lua_State* L, int index, const std::string& field, int type, bool pop)
	{
		bool ret = true;

		// Push key and get field
		lua_pushstring(L, field.c_str());
		if (lua_rawget(L, index < 0 ? (index - 1) : index) != type)
			ret = false;

		if (pop)
			lua_pop(L, 1);

		return ret;
	}

	bool lua_constraint_string_equal(lua_State* L, int index, const std::string& string)
	{
		std::string str_value = lua_tostring(L, index);

		return str_value == string;
	}

	bool lua_constraint_is_of_type(lua_State* L, int index, int type)
	{
		return lua_type(L, index) == type;
	}

}