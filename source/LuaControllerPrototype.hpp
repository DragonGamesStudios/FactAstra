#pragma once

#include "LuaPrototype.hpp"

class fa_LuaControllerPrototype : public fa_LuaPrototype
{
public:
	typedef std::shared_ptr<fa_LuaControllerPrototype> Ptr;
	typedef std::shared_ptr<const fa_LuaControllerPrototype> ConstPtr;

	fa_LuaControllerPrototype(lua_State* L, fa_Error* error);
	virtual ~fa_LuaControllerPrototype() = default;

	virtual void push(lua_State * L) override;

	virtual type_id_type get_type_id() const override;
	virtual const std::string& get_type_name() const override;

	static const std::string controller_type_name;
	static constexpr type_id_type controller_type_id = 2;
};