#include "LuaStorage.hpp"
#include "util.hpp"

#include <set>

template<class T>
static void get_prototypes(fa_PrototypeSet::MappedPtr<const fa_LuaPrototype>& prototypes, const fa_PrototypeSet::MappedPtr<T>& source)
{
	for (const auto& [name, ptr] : source)
	{
		prototypes.insert({ name, std::static_pointer_cast<const fa_LuaPrototype>(ptr) });
	}
}

template<class T>
static fa_LuaPrototype::ConstPtr get_root_prototype(const std::string& name, const fa_PrototypeSet::MappedPtr<T>& source)
{
	auto it = source.find(name);

	if (it == source.end())
		return 0;
	else
		return it->second;
}

fa_LuaStorage::fa_LuaStorage()
{
	storage_state = 0;
	current_mod = 0;
}

fa_LuaStorage::~fa_LuaStorage()
{
	close_storage_state();
}

void fa_LuaStorage::push(lua_State* L)
{
	lua_newtable(L);

	fa_util::lua_push_function(L, std::bind(&fa_LuaStorage::lua_add, this, std::placeholders::_1));
	lua_setfield(L, -2, "add");

	fa_util::lua_push_function(L, std::bind(&fa_LuaStorage::lua_extend, this, std::placeholders::_1));
	lua_setfield(L, -2, "extend");

	fa_util::lua_push_function(L, std::bind(&fa_LuaStorage::lua_get_name_list, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_name_list");

	fa_util::lua_push_function(L, std::bind(&fa_LuaStorage::lua_get_type_list, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_type_list");

	fa_util::lua_push_function(L, std::bind(&fa_LuaStorage::lua_get, this, std::placeholders::_1));
	lua_setfield(L, -2, "get");
}

fa_Error fa_LuaStorage::load()
{
	fa_Error error;
	std::string type;

	// Iterate over types
	lua_pushnil(storage_state);
	while (lua_next(storage_state, 1))
	{
		type = lua_tostring(storage_state, -2);
		auto prototype_id = prototype_types.at(type);
		const auto& histories = prototype_history.at(type);

		// Iterate over prototypes
		lua_pushnil(storage_state);
		while (lua_next(storage_state, -2))
		{
			fa_LuaPrototype::Ptr ptr;

			// Prototype currently on top
			switch (prototype_id)
			{
			case fa_LuaControllerPrototype::controller_type_id:
				ptr = std::make_shared<fa_LuaControllerPrototype>(storage_state, &error);
				prototypes.controller.insert({ ptr->get_name(), std::dynamic_pointer_cast<fa_LuaControllerPrototype>(ptr) });
				break;

			default:
				error.code = fa_errno::lua_invalid_prototype;
				error.description = "Prototype type not supported.";
			}

			if (error.code != fa_errno::ok)
				break;

			std::string name = lua_tostring(storage_state, -2);
			ptr->set_history(histories.at(name));

			// Pop
			lua_pop(storage_state, 1);
		}

		lua_pop(storage_state, 1);
	}

	return error;
}

void fa_LuaStorage::create_storage_state()
{
	storage_state = luaL_newstate();
	lua_newtable(storage_state);

	for (const auto& type_name : prototype_types)
	{
		lua_newtable(storage_state);
		lua_setfield(storage_state, -2, type_name.first.c_str());

		prototype_history.insert({ type_name.first, {} });
	}
}

void fa_LuaStorage::close_storage_state()
{
	if (storage_state)
	{
		lua_close(storage_state);
		storage_state = 0;
	}
}

void fa_LuaStorage::set_current_mod(const fa_LoadedMod* mod)
{
	current_mod = mod;
}

void fa_LuaStorage::clear()
{
	prototype_history.clear();
	prototypes.controller.clear();
}

void fa_LuaStorage::find_prototypes(std::map<fa_LuaPrototype::type_id_type, fa_PrototypeSet::MappedPtr<const fa_LuaPrototype>>& _prototypes, fa_LuaPrototype::type_id_type base, bool recursive) const
{
	std::map<fa_LuaPrototype::type_id_type, fa_PrototypeSet::MappedPtr<const fa_LuaPrototype>>::iterator it = _prototypes.insert({ base, {} }).first;

	switch (base)
	{
	case fa_LuaControllerPrototype::controller_type_id:
		get_prototypes(it->second, prototypes.controller);
		break;
	}

	if (recursive)
	{
		for (const auto& b : prototype_tree.at(base))
			find_prototypes(_prototypes, b, recursive);
	}
}

fa_LuaPrototype::ConstPtr fa_LuaStorage::get_prototype(fa_LuaPrototype::type_id_type type, const std::string& name)
{
	fa_LuaPrototype::ConstPtr ptr;

	switch (type)
	{
	case fa_LuaControllerPrototype::controller_type_id:
		ptr = get_root_prototype(name, prototypes.controller);
		break;
	}

	return ptr;
}

int fa_LuaStorage::lua_extend(lua_State* L)
{
	if (lua_gettop(L))
	{
		if (fa_util::lua_constraint_is_of_type(L, 1, LUA_TTABLE))
		{
			for (int i = 1; i <= lua_rawlen(L, 1); i++)
			{
				// Push prototype table
				lua_pushinteger(L, i);
				lua_rawget(L, 1);

				lua_add_m1(L);
			}
		}
		else
			luaL_error(L, "Invalid value for #1 (prototype array). Table expected.");
	}
	else
		luaL_error(L, "Missing value for #1 (prototype array).");

	return 0;
}

int fa_LuaStorage::lua_add(lua_State* L)
{
	if (lua_gettop(L))
	{
		lua_settop(L, 1);

		lua_add_m1(L);
	}
	else
		luaL_error(L, "Missing value for #1 (prototype).");

	return 0;
}

int fa_LuaStorage::lua_get_type_list(lua_State* L)
{
	lua_newtable(L);
	int i = 1;

	for (const auto& type_name : prototype_types)
	{
		lua_pushstring(L, type_name.first.c_str());
		lua_seti(L, -2, i++);
	}

	return 1;
}

int fa_LuaStorage::lua_get_name_list(lua_State* L)
{
	if (lua_gettop(L))
	{
		if (fa_util::lua_constraint_is_of_type(L, 1, LUA_TSTRING))
		{
			// Read type
			const char* type_name = lua_tostring(L, 1);

			// Fetch table
			lua_getfield(storage_state, 1, type_name);

			if (lua_isnil(storage_state, -1))
			{
				lua_pop(storage_state, 1);
				luaL_error(L, "Given type is not a valid type.");
			}

			// Create&copy
			int i = 1;
			lua_newtable(L);

			lua_pushnil(storage_state);
			while (lua_next(storage_state, -2))
			{
				const char* k = lua_tostring(storage_state, -2);
				lua_seti(L, -2, i++);

				lua_pop(storage_state, 1);
			}
		}
		else
			luaL_error(L, "Invalid value for #1 (type name). String expected.");
	}
	else
		luaL_error(L, "Missing value for #1 (type name).");

	return 1;
}

int fa_LuaStorage::lua_get(lua_State* L)
{
	if (lua_gettop(L) >= 2)
	{
		const char* type_value = 0;
		const char* name_value = 0;

		if (fa_util::lua_constraint_is_of_type(L, 1, LUA_TSTRING))
		{
			type_value = lua_tostring(L, 1);
		}
		else
			luaL_error(L, "Invalid value for #1 (type). String expected.");

		if (fa_util::lua_constraint_is_of_type(L, 2, LUA_TSTRING))
		{
			name_value = lua_tostring(L, 2);
		}
		else
			luaL_error(L, "Invalid value for #2 (name). String expected.");

		// Get the right table on storage stack
		lua_getfield(storage_state, 1, type_value);

		if (lua_isnil(storage_state, -1))
		{
			lua_pop(storage_state, 1);
			luaL_error(L, "Invalid value for #1 (type). Given type does not exist.");
		}

		lua_getfield(storage_state, -1, name_value);

		// Move the prototype field
		fa_util::lua_move_value(storage_state, L, -1, 0);

		lua_pop(storage_state, 2);
	}
	else
		luaL_error(L, "Missing value for #1 (type) or #2 (name).");

	return 1;
}

int fa_LuaStorage::lua_add_m1(lua_State* L)
{
	if (fa_util::lua_constraint_is_of_type(L, -1, LUA_TTABLE))
	{
		// Get type and name

		if (!fa_util::lua_constraint_has_field_of_type(L, -1, "type", LUA_TSTRING, false))
		{
			luaL_error(L, "Missing field `type` at ROOT.");
		}

		const char* type_value = lua_tostring(L, -1);
		lua_pop(L, 1);

		if (!fa_util::lua_constraint_has_field_of_type(L, -1, "name", LUA_TSTRING, false))
		{
			luaL_error(L, "Missing field `name` at ROOT.");
		}

		const char* name_value = lua_tostring(L, -1);
		lua_pop(L, 1);

		// Get the right table on storage stack
		lua_getfield(storage_state, 1, type_value);

		if (lua_isnil(storage_state, -1))
			luaL_error(L, "Invalid prototype type. Given type does not exist.");

		// Set the right field
		fa_util::lua_move_value(L, storage_state, -1, 0);
		lua_setfield(storage_state, -2, name_value);

		// Pop table
		lua_pop(storage_state, 1);

		if (current_mod)
		{
			std::map<std::string, std::vector<const fa_LoadedMod*>>::iterator it = prototype_history.at(type_value).insert({ name_value, {} }).first;

			it->second.push_back(current_mod);
		}
	}
	else
		luaL_error(L, "Invalid value for #1 (prototype). Table expected.");

	return 0;
}
