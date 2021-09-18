#pragma once

#include <lua/lua.hpp>

class fa_LuaObject
{
public:
	fa_LuaObject();
	virtual ~fa_LuaObject() = default;

	virtual void push(lua_State* L) = 0;
};