# Mod structure

A mod is either a directory or a .zip file. Zip files may have their structure nested inside another directory with the same name.

Every mod must contain info.json file.

Every mod may contain the following files:

* `settings.lua`
* `settings-updates.lua`
* `settings-final-fixes.lua`
* `data.lua`
* `data-updates.lua`
* `data-final-fixes.lua`
* `control.lua`

Every mod may contain a directory with scenarios.

## `info.json`

This file contains basic information about the mod. If it's missing, the mod file/directory is ignored. info.json contains the following fields:

|Field|Type|Optional|Description|
|-|-|-|-|
|`name`|`string`|No|The mod's name used in Lua.|
|`title`|`string`|No|The mod's title visible is mods GUI in the game.|
|`version`|`version string`|No|The mod's version in the form of `major.minor.patch`|
|`factastra_version`|`version string`|No|The game version in the form of `major.minor.patch` this mod works on.|
|`description`|`string`|Yes|Description of the mod.|

# Scenario structure
TODO