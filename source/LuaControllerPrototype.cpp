#include "LuaControllerPrototype.hpp"

const std::string fa_LuaControllerPrototype::controller_type_name = "controller";

fa_LuaControllerPrototype::fa_LuaControllerPrototype(lua_State* L, fa_Error* error)
	: fa_LuaPrototype(L, error)
{
}

void fa_LuaControllerPrototype::push(lua_State* L)
{
	fa_LuaPrototype::push(L);
}

fa_LuaPrototype::type_id_type fa_LuaControllerPrototype::get_type_id() const
{
	return controller_type_id;
}

const std::string& fa_LuaControllerPrototype::get_type_name() const
{
	return controller_type_name;
}
