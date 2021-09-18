#include "LuaPrototype.hpp"
#include "util.hpp"
#include "App.hpp"

const std::string fa_LuaPrototype::prototype_type_name = "prototype";

static uint64_t prototype_id = 0;

static std::string get_prototype_string(const fa_LuaPrototypeDescription& description);

fa_LuaPrototype::fa_LuaPrototype(lua_State* L, fa_Error* error)
{
	error->code = fa_errno::lua_invalid_prototype;
	id = ++prototype_id;

	// Table parsing

	// {
	// 	   name: string (man.),
	// 	   type: string (man.)
	// }

	// Base table check
	if (!fa_util::lua_constraint_is_of_type(L, -1, LUA_TTABLE))
	{
		error->description = "Invalid prototype. Prototype is not a table.";
		return;
	}

	// `type` check can be skipped

	// `name` check
	if (!fa_util::lua_constraint_has_field_of_type(L, -1, "name", LUA_TSTRING, false))
	{
		error->description = "ROOT.name missing/of wrong type. String expected.";
		return;
	}

	name = lua_tostring(L, -1);
	lua_pop(L, 1);

	history_description.first.type = fa_FieldType::literal;
	history_description.first.type_id = FACTASTRA_LITERAL_STRING;

	add_description_field({ "name", fa_FieldType::literal, FACTASTRA_LITERAL_STRING, &name });
	add_description_field({ "id", fa_FieldType::literal, FACTASTRA_LITERAL_UINT64, &id });
	add_description_field({ "history", fa_FieldType::literal, FACTASTRA_LITERAL_VECTOR, &history_description });

	error->code = fa_errno::ok;
}

void fa_LuaPrototype::push(lua_State* L)
{
	// {
	// 	   name: string,
	// 	   type: string,
	// 	   id: uint64
	// }

	lua_newtable(L);

	fa_util::lua_push_function(L, std::bind(&fa_LuaPrototype::lua_get_type_name, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_type_name");

	fa_util::lua_push_function(L, std::bind(&fa_LuaPrototype::lua_get_name, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_name");

	fa_util::lua_push_function(L, std::bind(&fa_LuaPrototype::lua_get_id, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_id");

	fa_util::lua_push_function(L, std::bind(&fa_LuaPrototype::lua_get_history, this, std::placeholders::_1));
	lua_setfield(L, -2, "get_history");
}

fa_LuaPrototype::type_id_type fa_LuaPrototype::get_type_id() const
{
	return prototype_type_id;
}

const std::string& fa_LuaPrototype::get_type_name() const
{
	return prototype_type_name;
}

uint64_t fa_LuaPrototype::get_id() const
{
	return id;
}

const std::string& fa_LuaPrototype::get_name() const
{
	return name;
}

const std::vector<const fa_LoadedMod*>& fa_LuaPrototype::get_history() const
{
	return history;
}

const fa_LuaPrototypeDescription& fa_LuaPrototype::get_description() const
{
	return description;
}

std::string fa_LuaPrototype::get_string() const
{
	return get_prototype_string(description);
}

void fa_LuaPrototype::add_description_field(const fa_LuaPrototypeFieldDescription& field)
{
	description.insert({ field.name, field });
}

void fa_LuaPrototype::set_history(const std::vector<const fa_LoadedMod*>& _history)
{
	history = _history;

	history_description.second.clear();

	for (const auto& mod : history)
		history_description.second.push_back((void*)&mod->name);
}

int fa_LuaPrototype::lua_get_type_name(lua_State* L)
{
	lua_pushstring(L, get_type_name().c_str());
	return 1;
}

int fa_LuaPrototype::lua_get_name(lua_State* L)
{
	lua_pushstring(L, get_name().c_str());
	return 1;
}

int fa_LuaPrototype::lua_get_id(lua_State* L)
{
	lua_pushinteger(L, get_id());
	return 1;
}

int fa_LuaPrototype::lua_get_history(lua_State* L)
{
	lua_newtable(L);

	for (int i = 0; i < history.size(); i++)
	{
		lua_pushstring(L, history[i]->name.c_str());
		lua_seti(L, -2, (long long)i + 1);
	}

	return 1;
}

static std::string get_literal_string(fa_LuaPrototype::type_id_type type, void* value)
{
	std::string s;
	std::pair<fa_LuaPrototypeFieldDescription, std::vector<void*>>* p;
	fa_LuaPrototypeFieldDescription f_description;

	switch (type)
	{
	case FACTASTRA_LITERAL_STRING:
		s = *((std::string*)value);
		break;

	case FACTASTRA_LITERAL_UINT64:
		s = std::to_string(*((uint64_t*)value));
		break;

	case FACTASTRA_LITERAL_VECTOR:
		p = (std::pair<fa_LuaPrototypeFieldDescription, std::vector<void*>>*)value;
		f_description = p->first;

		s.push_back('{');

		for (int i = 0; i < p->second.size(); i++)
		{
			f_description.name = std::to_string(i);
			f_description.value = p->second[i];
			s += f_description.str() + ", ";
		}

		s.push_back('}');
		break;

	default:
		break;
	}

	return s;
}

static std::string get_prototype_string(const fa_LuaPrototypeDescription& description)
{
	std::string s;

	s += "{\n";
	for (const auto& [name, field] : description)
	{
		s += name + " = " + field.str() + ",\n";
	}
	s.push_back('}');

	return s;
}

std::string fa_LuaPrototypeFieldDescription::str() const
{
	std::string s;

	switch (type)
	{
	case fa_FieldType::literal:
		s += get_literal_string(type_id, value);
		break;

	case fa_FieldType::prototype:
		s += get_prototype_string((*((fa_LuaPrototype::ConstPtr*)value))->get_description());
		break;
	}

	return s;
}
