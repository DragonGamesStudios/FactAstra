#pragma once

#include <lua/lua.hpp>

#include <map>
#include <vector>
#include <string>
#include <functional>

class fa_LuaStateManager
{
public:
	typedef std::function<void(lua_State*, void*)> stack_func;

	fa_LuaStateManager();

	void create_state(void* user_data, void* cleanup_user_data);
	void delete_state(void* user_data);

	void set_prepare_function(const stack_func& func);
	const stack_func& get_prepare_function() const;

	void set_cleanup_function(const stack_func& func);
	const stack_func& get_cleanup_function() const;

	void execute_function(const stack_func& func, void* user_data);

private:
	lua_State* current_state;
	stack_func prepare_function;
	stack_func cleanup_function;
};