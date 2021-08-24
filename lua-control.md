- [Classes](#classes)
    - [`LuaEntity`](#luaentity)
        - [`get_inventory()`]
        - [`get_infinite_filters()`](#get_infinite_filters)
        - [`get_item()`](#get_item)
        - [`get_items()`](#get_items)
        - [`get_main_inventory()`](#get_main_inventory)
        - [`get_position()`](#get_position)
        - [`get_prototype()`](#get_prototype)
        - [`get_surface()`](#get_surface)
        - [`has_infinite_filters()`](#has_infinite_filters)
        - [`has_inventory()`](#has_inventory)
        - [`is_infinite_container()`](#is_infinite_container)
        - [`is_transport_line()`](#is_transport_line)
        - [`pop_end()`](#pop_end)
        - [`push_begin`](#push_begin)
        - [`set_infinite_filters()`](#set_infinite_filters)
        - [`set_mode()`](#set_mode)
        - [`set_position()`](#set_position)
    - [`LuaEventControl`](#luaeventcontrol)
        - [`on_event()`](#on_event)
        - [`on_session_ended()`](#on_session_ended)
        - [`on_session_started()`](#on_session_started)
    - [`LuaGameControl`](#luagamecontrol)
        - [`get_host()`](#get_host)
        - [`get_player()`](#get_player)
        - [`get_players()`](#get_players)
        - [`get_surface()`](#get_surface)
        - [`get_surfaces()`](#get_surfaces)
    - [`LuaInventory`](#luainventory)
        - [`clear`](#clear)
        - [`get_contents`](#get_contents)
        - [`get_item_count`](#get_item_count)
        - [`get_stacks`](#get_stacks)
        - [`insert`](#insert)
        - [`remove`](#remove)
        - [`set_size`](#set_size)
    - [`LuaPhys`](#luaphys)
        - [`dot()`](#dot)
        - [`norm()`](#norm)
        - [`rotate()`](#rotate)
        - [`scale()`](#scale)
        - [`translate()`](#translate)
    - [`LuaPlayer`](#luaplayer)
        - [`get_position()`](#get_position)
        - [`get_surface()`](#get_surface)
        - [`get_zoom()`](#get_zoom)
        - [`set_position()`](#set_position)
        - [`set_zoom()`](#set_zoom)
    - [`LuaPrototype`](#luaprototype)
        - [`derives_from()`](#derives_from)
        - [`is_valid()`](#is_valid)
    - [`LuaSurface`](#luasurface)
        - [`create_entity()`](#create_entity)
        - [`find_entities()`](#find_entities)
        - [`find_entities_filtered()`](#find_entities_filtered)
    - [`LuaVec2`](#luavec2)
        - [`length()`](#length)
        - [`x()`](#x)
        - [`y()`](#y)

# Classes

## `LuaEntity`

**Functions**

### `get_inventory`

*Description*

Gets the specified inventory of an entity.

*Usage*

```lua
local inventory = entity.get_inventory(inventory_id)
```

*Arguments*

|Name|Type|Description|
|-|-|-|
|`inventory_id`|[`defines.inventory.?`](#inventory)|Which inventory to return.|

*Returns*

[`LuaInventory`](#luainventory) instance. `nil` if the entity has no such inventory.

### `get_infinite_filters`

### `get_item`

### `get_items`

### `get_main_inventory`

### `get_position`

### `get_prototype`

### `get_surface`

### `has_infinite_filters`

### `has_inventory`

### `is_infinite_container`

### `is_transport_line`

### `pop_end`

### `push_begin`

### `set_infinite_filters`

### `set_mode`

### `set_position`

## `LuaEventControl`

**Description**

Provides control over the game events. Available in `control.lua` as `script`.

**Functions**

### `on_event`

*Description*

Registers a function directly tied to an event. For each mod only one function can be tied to each event.

*Usage*

```lua
script.on_event(event, fn)
```

*Arguments*

`event` (`defines.event.?`) : The event to tie this function to.
`fn` (`function void(event)`) : The function to call when the event occurs. It will receive one argument: an [event table](#events). No return values are expected.

### `on_session_ended`

*Description*

Registers a function that is run right before the active session ends.

*Usage*

```lua
script.on_session_ended(fn)
```

*Arguments*

`fn` (`function void()`) : The function to call when session ends. It won't receive any arguments. No return values are expected.

### `on_session_started`

*Description*

Registers a function that is run right after the active session starts.

*Usage*

```lua
script.on_session_started(fn)
```

*Arguments*

`fn` (`function void()`) : The function to call when session starts. It won't receive any arguments. No return values are expected. At the point it's executing, `game` is already available.

## `LuaGameControl`

**Description**

Provides control over all the session data. Available in `control.lua` as `game`.

**Functions**

### `get_host`

*Description*

Returns the player hosting the game.

*Usage*

```lua
local host = game.get_host()
```

*Returns*

An instance of [`LuaPlayer`](#luaplayer).

### `get_player`

*Description*

Returns the player with the specified index (starting from 1) in the session. If no index is given, the player directly related to that specific instance is returned.

*Usage*

```lua
local player = game.get_player(index)
```

*Arguments*

`index` (`uint32`, optional) : The player index. If `nil`, function returns the player directly related to the instance.

*Returns*

An instance of [`LuaPlayer`](#luaplayer).

### `get_players`

*Description*

Returns all the players in the session.

*Usage*

```lua
local players = game.get_players()
```

*Returns*

An array of [`LuaPlayer`](#luaplayer).

### `get_surface`

*Description*

Returns the surface with the specified index (starting from 1) existing in the session.

*Usage*

```lua
local surface = game.get_surface(index)
```

*Arguments*

`index` (`uint32`) : The surface index.

*Returns*

An instance of [`LuaSurface`](#luasurface).

### `get_surfaces`

*Description*

Returns all the surfaces existing in the session.

*Usage*

```lua
local surfaces = game.get_surfaces()
```

*Returns*

An array of [`LuaSurface`](#luasurface).

## `LuaInventory`

**Functions**

### `clear`

### `get_contents`

### `get_item_count`

### `get_size`

### `get_stacks`

### `insert`

### `remove`

### `set_size`

## `LuaPhys`

**Description**

Gives access to functions for things like vector math and physics calculations. Available using `phys`.

**Functions**

### `dot`

*Description*

Returns a dot product of given two vectors.

```
dot(a, b) = a.x * b.x + a.y * b.y
dot(a, b) = ||a|| * ||b|| * cos(theta)
```

Where `theta` is the angle between `a` and `b` and `||v||` is the magnitude of vector `v`.

*Usage*

```lua
local product = phys.dot(a, b)
```

*Arguments*

`a` ([`LuaVec2`](#luavec2)) : First vector.
`b` ([`LuaVec2`](#luavec2)) : Second vector.

*Returns*

The dot product of `a` and `b`.

### `norm`

*Description*

Returns normalized vector. Normalized vectors are also called unit vectors.

```
||norm(v)|| = 1
```

Where `||v||` is the magnitude of `v`.

*Usage*

```lua
local normalized = phys.norm(v)
```

*Arguments*

`v` ([`LuaVec2`](#luavec2)) : Vector to normalize.

*Returns*

Normalized vector `v`.

### `rotate`

*Description*

Rotates the vector counter-clockwise by the given angle.

*Usage*

```lua
local rotated = phys.rotate(vec, by)
```

*Arguments*

`vec` ([`LuaVec2`](#luavec2)) : Vector to rotate.
`by` ([`LuaHighpFloat`](#luahighpfloat)) : Angle in radians to rotate `v` by.

*Returns*

The rotated vector `vec`.

### `scale`

*Description*

Scales the vector by the given values.

*Usage*

```lua
local scaled = phys.scale(vec, by)
```

*Arguments*

`vec` ([`LuaVec2`](#luavec2)) : Vector to scale.
`by` ([`LuaVec2`](#luavec2)) : Values to scale `v` by.

*Returns*

The scaled vector `vec`.

### `translate`

*Description*

Translates the vector by the given values.

*Usage*

```lua
local translated = phys.translate(vec, by)
```

*Arguments*

`vec` ([`LuaVec2`](#luavec2)) : Vector to translate.
`by` ([`LuaVec2`](#luavec2)) : Values to translate `v` by.

*Returns*

The translated vector `vec`.

### `vec`

*Description*

Creates a new vector based on the given vector representation.

*Usage*

```lua
local vec1 = phys.vec({x1, y1})
local vec2 = phys.vec({x = x2, y = y2})
local vec3 = phys.vec(vec1)
```

*Arguments*

`[1]` : Vector representation (as described in [`LuaVec2`](#luavec2)). If given a [`LuaVec2`](#luavec2) instance, the function will copy it.

*Returns*

A new [`LuaVec2`](#luavec2) instance.

## `LuaPlayer`

**Functions**

### `get_position`

### `get_surface`

### `get_zoom`

### `set_position`

### `set_zoom`

## `LuaPrototype`

**Functions**

### `derives_from`

### `is_valid`

## `LuaSurface`

**Description**

Every spaceship, every planet, every moon and every asteroid is a seperate surface. These surfaces have their own limited size, seperate coordinate systems and specific tiled worlds. This class describes them. Every surface is an istance of this class.

**Functions**

### `create_entity`

*Description*

Creates an entity if possile to place using the given metadata.

*Usage*

```lua
local err = surface.create_entity({position = ..., direction = ..., name = ...})
```

*Arguments*

`1` : Table with the following fields:

|Field|Type|Default|Description|
|-|-|-|-|
|`position`|[`LuaVec2`](#luavec2)|`{0,0}`|The entity position.|
|`direction`|`defines.direciton.?`|`defines.direciton.up`|The orientation of the entity.|
|`name`|`string`|-|The name of the entity prototype. Must be provided.|

*Returns*

Integer specifying if the entity was successfully placed.
`defines.place_error.ok` means no error occured.
`defines.place_error.terrain_mask` means the requested position was colliding with a tile with terrain mask the entity couldn't be placed on.
`defines.place_error.collision_mask` means the requested position was occupied by another entity with the same collision mask.

*Notes*

At least one of `area` and `radius`+`position` has to be filled in the table.

### `find_entities`

*Description*

Finds all entities in the given area specified by top-left and bottom-right points.

*Usage*

```lua
local entities = surface.find_entities(area)
```

*Arguments*

`area` : Table with the following structure: `{topleft = LuaVec2/{x = ..., y = ...}, bottomright = LuaVec2/{x = ..., y = ...}}`

*Returns*

An array of [`LuaEntity`](#luaentity).

### `find_entities_filtered`

*Description*

Finds filtered entities in the given area/radius.

*Usage*

```lua
local entities surface.find_entities_filtered({area = ..., position = ..., radius = ..., type = ..., basetype = ..., name = ...})
```

*Arguments*

`1` : Table with the following optional fields:

|Field|Type|Default|Description|
|-|-|-|-|
|`area`|`table`|Ignored|Table with the following structure: `{topleft = LuaVec2/{x = ..., y = ...}, bottomright = LuaVec2/{x = ..., y = ...}}`.|
|`position`|[`LuaVec2`](#luavec2)|`{0,0}` |When `radius` is given, this point becomes its origin.|
|`radius`|`number`|Ignored|Maximum distance of entity's position to `point`.|
|`type`|`string`|Ignored|The type an entity has to be of to be included.|
|`basetype`|`string`|`"base"`|The entity's type has to either derive from it or be it for the entity to be included.|
|`name`|`string`|Ignored|The name an entity has to have to be included.|

*Returns*

An array of [`LuaEntity`](#luaentity).

*Notes*

At least one of `area` and `radius`+`position` has to be filled in the table.

## `LuaVec2`

**Description**

This class describes vectors with x and y components. It supports most mathematical operations, either between two vectors or between a vector and a scalar. It can be created using [`phys.vec()`](#vec). Note that all functions that take in `LuaVec2` instances also accept vector representations. A vector representation is a table with two possible structures:
* `{x, y}`
* `{x = _x, y = _y}`

**Functions**

### `length`

*Description*

Returns the magnitude of the vector using the pythagorean theorem.

*Usage*

```lua
local len = vec.length()
```

*Returns*

The magnitude of the vector.

### `x`

*Description*

Returns the x component of the vector.

*Usage*

```lua
local x = vec.x()
```

*Returns*

Vector's x component.

### `y`

*Description*

Returns the y component of the vector.

*Usage*

```lua
local y = vec.y()
```

*Returns*

Vector's y component.

# Events

# Defines

# `control.lua` pipeline

* Start session
* Initialize `LuaEventControl`
* Execute `control.lua`
* Initialize `LuaGameControl`
* `on_session_started()`
* load selected save
* Initialize all surfaces, systems and entities.
* `on_load()`
* All events happen here
* `on_session_ended()`
* End session