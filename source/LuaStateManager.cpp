#include "LuaStateManager.hpp"

static void default_stack_func(lua_State* L, void* udata)
{
}

fa_LuaStateManager::fa_LuaStateManager()
{
	current_state = 0;
	cleanup_function = default_stack_func;
	prepare_function = default_stack_func;
}

void fa_LuaStateManager::create_state(void* user_data, void* cleanup_user_data)
{
	if (current_state)
	{
		cleanup_function(current_state, cleanup_user_data);
		lua_close(current_state);
	}

	current_state = luaL_newstate();

	prepare_function(current_state, user_data);
}

void fa_LuaStateManager::delete_state(void* user_data)
{
	if (current_state)
	{
		cleanup_function(current_state, user_data);
		lua_close(current_state);
		current_state = 0;
	}
}

void fa_LuaStateManager::set_prepare_function(const stack_func& func)
{
	prepare_function = func;
}

const fa_LuaStateManager::stack_func& fa_LuaStateManager::get_prepare_function() const
{
	return prepare_function;
}

void fa_LuaStateManager::set_cleanup_function(const stack_func& func)
{
	cleanup_function = func;
}

const fa_LuaStateManager::stack_func& fa_LuaStateManager::get_cleanup_function() const
{
	return cleanup_function;
}

void fa_LuaStateManager::execute_function(const stack_func& func, void* user_data)
{
	if (current_state)
		func(current_state, user_data);
}
