- [Classes](#classes)
    - [`LuaControllerPrototype`](#luacontrollerprototype)
    - [`LuaPrototype`](#luaprototype)
        - [`get_history()`](#get_history)
        - [`get_id()`](#get_id)
        - [`get_name()`](#get_name)
        - [`get_type_name()`](#get_type_name)

# Classes

## `LuaPrototype`

Base class for all prototypes.

**Respective prototype**: `prototype<abstract>`<br>
**Type name**: `prototype`<br>
**Parent prototype**: -

**Functions**

### `get_history`

*Description*

Gets the mod history of the prototype.

*Usage*

```lua
local history = prototype.get_history()
```

*Returns*

An `array` of `strings` specifying the prototype's mod history. Every entry is a mod name.

### `get_id`

*Description*

Gets the unique id of the prototype.

*Usage*

```lua
local id = prototype.get_id()
```

*Returns*

An `uint64` specifying the prototype's id.

### `get_name`

*Description*

Gets the name of the prototype.

*Usage*

```lua
local name = prototype.get_name()
```

*Returns*

A `string` specifying the prototype's name.

### `get_type_name`

*Description*

Gets the type name of the prototype.

*Usage*

```lua
local type_name = prototype.get_type_name()
```

*Returns*

A `string` specifying the prototype's type.

## `LuaControllerPrototype`

Controller gives the player control over the game. It specifies the zoom bounds and controls.

**Respective prototype**: `controller`<br>
**Type name**: `controller`<br>
**Parent prototype**: [`LuaPrototype`](#luaprototype)