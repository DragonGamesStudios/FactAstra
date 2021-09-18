#pragma once
#define FACTASTRA_LITERAL_UINT64 0
#define FACTASTRA_LITERAL_STRING 1
#define FACTASTRA_LITERAL_VECTOR 2

#include "LuaObject.hpp"
#include "errors.hpp"

#include <string>
#include <map>
#include <vector>
#include <memory>

enum class fa_FieldType
{
	prototype,
	concept,
	literal
};

class fa_LuaPrototype;

struct fa_LuaPrototypeFieldDescription
{
	std::string name;
	fa_FieldType type = fa_FieldType::literal;
	uint64_t type_id = 0;
	void* value = 0;

	std::string str() const;
};

typedef std::map<std::string, fa_LuaPrototypeFieldDescription> fa_LuaPrototypeDescription;

struct fa_LoadedMod;

class fa_LuaPrototype : public fa_LuaObject
{
public:
	typedef uint64_t type_id_type;
	typedef std::shared_ptr<fa_LuaPrototype> Ptr;
	typedef std::shared_ptr<const fa_LuaPrototype> ConstPtr;

	fa_LuaPrototype(lua_State* L, fa_Error* error);
	virtual ~fa_LuaPrototype() = default;

	virtual void push(lua_State* L) override;

	virtual type_id_type get_type_id() const;
	virtual const std::string& get_type_name() const;

	uint64_t get_id() const;

	const std::string& get_name() const;

	const std::vector<const fa_LoadedMod*>& get_history() const;
	const fa_LuaPrototypeDescription& get_description() const;
	std::string get_string() const;

	static const std::string prototype_type_name;
	static constexpr type_id_type prototype_type_id = 1;

protected:
	void add_description_field(const fa_LuaPrototypeFieldDescription& field);

private:
	std::string name;
	uint64_t id;
	std::vector<const fa_LoadedMod*> history;
	fa_LuaPrototypeDescription description;

	std::pair<fa_LuaPrototypeFieldDescription, std::vector<void*>> history_description;

	void set_history(const std::vector<const fa_LoadedMod*>& _history);

	// Lua API
	int lua_get_type_name(lua_State* L);
	int lua_get_name(lua_State* L);
	int lua_get_id(lua_State* L);
	int lua_get_history(lua_State* L);

	friend class fa_LuaStorage;
};