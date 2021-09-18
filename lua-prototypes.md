- [Classes](#classes)
    - [`LuaStorage`](#luastorage)
        - [`add()`](#add)
        - [`extend()`](#extend)
        - [`get()`](#get)
        - [`get_name_list()`](#get_name_list)
        - [`get_type_list()`](#get_type_list)
- [Prototypes](#prototypes)
    - [`controller`](#controller)
    - [`prototype<<abstract>`](#prototype)
        - [`name`](#name)
        - [`type`](#type)
- [Concepts](#concepts)

# Classes

## `LuaStorage`

Maintains all the prototypes. Available in prototype step as `prototypes`.

**Functions**

### `add`

*Description*

Overwrites or creates a prototype.

*Usage*

```lua
prototypes.add(prototype)
```

*Arguments*

|Name|Type|Description|
|-|-|-|
|`prototype`|`table`|The prototype table.|

### `extend`

*Description*

Adds all the prototypes in an array.

*Usage*

```lua
prototypes.extend({
    prototype1,
    prototype2,
    prototype3
})
```

*Arguments*

|Name|Type|Description|
|-|-|-|
|`prototype_array`|`array` of `table`|An array of prototypes.|

### `get`

*Description*

Gets the specified prototype table.

*Usage*

```lua
local prototype = prototypes.get(type, name)
```

*Arguments*

|Name|Type|Description|
|-|-|-|
|`type`|`string`|The prototype's type.|
|`name`|`string`|The prototype's name.|

*Returns*

Prototype `table`. `nil` if the prototype does not exist. Using an invalid type will raise an error.

### `get_name_list`

*Description*

Gets an array of given type's prototype names.

*Usage*

```lua
local names = ptototypes.get_name_list(type)
```

*Arguments*

|Name|Type|Description|
|-|-|-|
|`type`|`string`|The prototype's type.|

*Returns*

`array` of `string`. Each entry represents one prototype's name. Using an invalid type will raise an error.

### `get_type_list`

*Description*

Gets a list of all prototype types;

*Usage*

```lua
local types = prototypes.get_type_list()
```

*Returns*

`array` of `string`. Each entry represents one type.

# Prototypes

## `controller`

**Respective prototype class**: `LuaControllerPrototype`<br>
**Parent prototype**: `prototype`

**Usage**

```lua
local prototype = {
    -- prototype
    type = "controller",
    name = "default",
    -- controller
}
```

**Fields**

## `prototype<abstract>`

**Respective prototype class**: `LuaPrototype`<br>
**Parent prototype**: -

**Usage**

```lua
local prototype = {
    -- prototype
    type = "prototype", -- WARNING: prototype is not a valid type. This is just to show what a prototype table looks like.
    name = "my-prototype",
}
```

**Fields**

*Mandatory*

### `name`

Specifies the prototype's name. All prototypes of one type must have different names.

### `type`

Specifies the prototype's type.

# Concepts