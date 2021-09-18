#pragma once

#include "LuaControllerPrototype.hpp"

#include <map>
#include <vector>
#include <memory>
#include <set>

// prototype
//   + controller

const std::map<fa_LuaPrototype::type_id_type, std::set<fa_LuaPrototype::type_id_type>> prototype_tree = {
	{ fa_LuaPrototype::prototype_type_id, { fa_LuaControllerPrototype::controller_type_id } },
	{ fa_LuaControllerPrototype::controller_type_id, {} },
};

const std::map<std::string, fa_LuaPrototype::type_id_type> prototype_types = {
	{ "prototype", fa_LuaControllerPrototype::prototype_type_id },
	{ "controller", fa_LuaControllerPrototype::controller_type_id }
};

struct fa_PrototypeSet
{
	template<class T>
	using MappedPtr = std::map<std::string, std::shared_ptr<T>>;

	MappedPtr<fa_LuaControllerPrototype> controller;
};

class fa_LuaStorage : public fa_LuaObject
{
public:
	fa_LuaStorage();
	virtual ~fa_LuaStorage();
	
	void push(lua_State* L) override;

	fa_Error load();

	void create_storage_state();
	void close_storage_state();

	void set_current_mod(const fa_LoadedMod* mod);

	void clear();

	// Utility
	void find_prototypes(std::map<fa_LuaPrototype::type_id_type, fa_PrototypeSet::MappedPtr<const fa_LuaPrototype>>& prototypes, fa_LuaPrototype::type_id_type base, bool recursive = true) const;
	fa_LuaPrototype::ConstPtr get_prototype(fa_LuaPrototype::type_id_type type, const std::string& name);

private:
	fa_PrototypeSet prototypes;

	lua_State* storage_state;
	const fa_LoadedMod* current_mod;

	std::map<std::string, std::map<std::string, std::vector<const fa_LoadedMod*>>> prototype_history;

	int lua_extend(lua_State* L);
	int lua_add(lua_State* L);
	int lua_get_type_list(lua_State* L);
	int lua_get_name_list(lua_State* L);
	int lua_get(lua_State* L);
	int lua_add_m1(lua_State* L);
};