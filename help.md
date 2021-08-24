# CMake options

You can change options of CMake build by adding `-D <option name>=<value>` before source path.

## 1. `CONFIGURATION`
Debug or Release. Specifies how to build the executable.

# Command line options

```cmd
FactAstra [-l <path>] [--window] [--console] [--modmanager] [--savemanager] [--dontload]
```

where

```cmd
FactAstra
```

is equivalent to

Linux:

```cmd
FactAstra -l "~/.local/DragonGames/Factastra/log.log" --window --console --modmanager --savemanager
```

Windows:

```cmd
FactAstra -l "%AppData%/DragonGames/FactAstra/log.log" --window --console --modmanager --savemanager
```

## 1. `--window`

**Description**: If present, the game window (graphical part) is run.<br>
**Default (no arguments)**: Active.

## 2. `--console`

**Description**: If present, console is enabled and allows controlling the game from it.<br>
**Default (no arguments)**: Active.

## 3. `--modmanager`

**Description**: If present, built-in mod manager (console) is run. This option is active automatically, when --console is present.<br>
**Default (no arguments)**: Active.

## 4. `--savemanager`

**Description**: If present, built-in save manager (console) is run. This option is active automatically, when --console is present.<br>
**Default (no arguments)**: Active.

## 5. `--dontload`

**Description**: If present, prototypes aren't loaded immediately after starting the game. Instead, the application waits for `prototype load` command.<br>
**Default**: Inactive.

## 6. `-l <path>`

**Description**: Specifies the path of log file.<br>
**Default (no arguments)**:<br>
*Windows*: `"%AppData%/DragonGames/FactAstra/log.log"`<br>
*Linux*: `"~/.local/DragonGames/Factastra/log.log"`

# Commands:

## 0. `quit`

```cmd
quit
```

Terminates the program. If there is an active session, a prompt will be shown, asking wether or not to save the game.
 
## 1. `modmanager`:

### 1.1. `list`

```cmd
modmanager list [-t (all|zip|dir)] [-f (all|enabled|disabled)] [-r <expression>]
```

where

```cmd
modmanager list
```

is equivalent to

```cmd
modmanager list -t all -f all -r .+
```

#### **Description**

Lists all mods using the given filters

**Options:**

1.1.1. `-t (all|zip|dir)`

*Description*: Filters the list by the type of mods (zip archives/pure directories)<br>
*Default*: `all`

1.1.2. `-f (all|active|inactive)`

*Description*: Filters the list by wether or not given mod is active.<br>
*Default*: `all`

1.1.3. `-r <expression>`

*Description*: Filters the list using the given regex string. Only the directory/archive name (not extension) is matched. The rest of the path is not.<br>
*Default*: `.+`

### 1.2. `checkvalid`

```cmd
modmanager checkvalid [<path>|<name>]
```

#### **Description**

Checks validity of all mods. If no path/name is given, checks all directories with valid structure for validity of their dependencies. If a name is given, checks the mod for validity of dependencies. If a path (name was not recognised) is given, checks the mod for validity of `info.json`, dependencies and file structure.

### 1.3. `info`

```cmd
modmanager info <path>
```

#### **Description**

Gives information about the specified mod. Information includes:
Mod's author, title and code name, dependencies, version and wether or not it's enabled.

### 1.4. `enable`

```cmd
modmanager enable [<path>] [-r <expression>]
```

where

```cmd
modmanager enable
```

is equivalent to

```cmd
modmanager enable -r .+
```

**Description**

Enables specific mods or filtered mods. If a path is given, the mod is enabled. If not, all mods are enabled after filtering with regex.

**Options:**

1.4.1. `-r <expression>`

*Description*: Filters the mods using the given regex string. Only the directory/archive name (not extension) is matched. The rest of the path is not.<br>
*Default*: `.+`

### 1.5. `disable`

```cmd
modmanager disable [<path>] [-r <expression>] [--includebase]
```

where

```cmd
modmanager disable
```

is equivalent to

```cmd
modmanager disable -r .+
```

**Description**

Disables specific mod or filtered mods. If a path is given, the mod is disabled. If not, all mods are disabled after filtering with regex.

*Warning: This command may disable base mod.*

**Options:**

1.5.1. `-r <expression>`

*Description*: Filters the mods using the given regex string. Only the directory/archive name (not extension) is matched. The rest of the path is not.<br>
*Default*: `.+`

1.5.2. `--includebase`

*Description*: Includes base mod into filters.

```
modmanager disable
```

Will not disable the base mod, as opposed to

```
modmanager disable --includebase
```

or

```
modmanager disable base
```

### 1.6. `saveconfig`

```cmd
modmanager saveconfig <path>
```

**Description**

Saves the current configuration in JSON format into the given file.

### 1.7. `loadconfig`

```cmd
modmanager loadconfig <path> [-o (or|and|xor|nor|nand|xnor|override)] [--negate]
```

where

```cmd
modmanager loadconfig <path>
```

is equivalent to

```cmd
modmanager loadconfig <path> -o override
```

**Description**

Loads the configuration from the given file. Options can specify how to merge it with the current one.

**Options:**

1.7.1. `-o (or|and|xor|nor|nand|xnor|override)`

*Description*: Defines how to merge the configuration with the current one. 

| or (loaded ->) | enabled | disabled | nonexistent |
|----------------|---------|----------|-------------|
| enabled        |    1    |     1    |      1      |
| disabled       |    1    |     0    |      0      |
| nonexistent    |    1    |     0    |      -      |

| and (loaded ->) | enabled | disabled | nonexistent |
|-----------------|---------|----------|-------------|
| enabled         |    1    |     0    |      1      |
| disabled        |    0    |     0    |      0      |
| nonexistent     |    1    |     0    |      -      |

| xor (loaded ->) | enabled | disabled | nonexistent |
|-----------------|---------|----------|-------------|
| enabled         |    0    |     1    |      1      |
| disabled        |    1    |     0    |      0      |
| nonexistent     |    1    |     0    |      -      |

| nor (loaded ->) | enabled | disabled | nonexistent |
|-----------------|---------|----------|-------------|
| enabled         |    0    |     0    |      1      |
| disabled        |    0    |     1    |      0      |
| nonexistent     |    1    |     0    |      -      |

| nand (loaded ->) | enabled | disabled | nonexistent |
|------------------|---------|----------|-------------|
| enabled          |    0    |     1    |      1      |
| disabled         |    1    |     1    |      0      |
| nonexistent      |    1    |     0    |      -      |

| xnor (loaded ->) | enabled | disabled | nonexistent |
|------------------|---------|----------|-------------|
| enabled          |    1    |     0    |      1      |
| disabled         |    0    |     1    |      0      |
| nonexistent      |    1    |     0    |      -      |

| override (loaded ->) | enabled | disabled | nonexistent |
|----------------------|---------|----------|-------------|
| enabled              |    1    |     0    |      1      |
| disabled             |    1    |     0    |      0      |
| nonexistent          |    1    |     0    |      -      |


*Default*: `override`

1.7.2. `--negate`

*Description*: If present, enabled mods are treated as disabled and *vice versa*.

### 1.8. `install`

```cmd
modmanager install <name> [-d (all|<integer>|none)]
```

**Description**

Installs given mod and (possibly) its dependencies.

**Options:**

1.8.1. `-d (all|<integer>|none)`

*Description*: Defines what dependencies to install. `all` means all, `none` means no dependencies are installed. If an integer is given, only dependencies to the specified depth are installed, where 1 means only direct dependencies.<br>
*Default*: `all`

### 1.9. `uninstall`

```cmd
modmanager uninstall <name> [-d (all|<integer>|none)]
```

**Description**

Uninstalls given mod and (possibly) mods that have a dependency on it.

**Options:**

1.9.1. `-d (all|<integer>|none)`

*Description*: Defines what dependent mods to uninstall. `all` means all, `none` means no dependent mods are uninstalled. If an integer is given, only mods dependent to the specified depth are installed, where 1 means only direct dependencies.<br>
*Default*: `none`

### 1.10. `pushmod`

```cmd
modmanager pushmod <path>
```

#### **Description**

Adds a mod to the configuration, if it's valid.

### 1.11. `pushdir`

```cmd
modmanager pushdir <path> [-t (all|zip|dir)]
```

where

```cmd
modmanager pushdir <path>
```

is equivalent to

```cmd
modmanager pushdir <path> -t all 
```

#### **Description**

Adds all valid mods in the given directory, potentially filtered by `-t` option.

**Options:**

1.11.1. `-t (all|zip|dir)`

*Description*: Filters the mods by their type (zip archives/pure directories)<br>
*Default*: `all`

### 1.12. `rmmod`

```cmd
modmanager rmmod <name>
```

#### **Description**

Removes given mod. This also adds it to ignored mods if its parent directory is included in additional directories. Re-adding the mod or removing the directory will remove it from the ignored mods.

### 1.13. `rmdir`

```cmd
modmanager rmdir <name>
```

#### **Description**

Removes given additional. This does not remove explicitly added mods (additional_mods) even if their parent directory is the one being removed.

## 2. `prototype`

### 2.1. `list`

```cmd
prototype list [-t <prototype type name>] [-r <expression>] [--recursive]
```

where

```cmd
prototype list
```

is equivalent to

```cmd
prototype list -t base -r .+ --recursive
```

#### **Description**

Lists all prototypes using the given filters.

**Options:**

2.1.1. `-t <prototype type name>`

*Description*: Specifies what type's prototypes to list.<br>
*Default*: `base`

2.1.2. `-r <expression>`

*Description*: Filters the prototypes using a regular expression.<br>
*Default*: `.+`

2.1.3. `--recursive`

*Description*: Defines wether or not also list prototypes of types derived from the given type.
*Default (no other args present)*: Active.

### 2.2. `load`

```cmd
modmanager load
```

#### **Description**

(Re)loads the mods and (re)parses the prototypes. Cannot be run when a game session is active.

### 2.3. `typeinfo`

```cmd
modmanager typeinfo <prototype type name> [--noinherit]
```

#### **Description**

Displays information about the given prototype type.

**Options:**

2.3.1. `--noinherit`

*Description*: If present, only directly defined fields are listed out.
*Default*: Inactive.

### 2.4. `protinfo`

```cmd
modmanager protinfo <prototype type name> <prototype name>
```

#### **Description**

Displays information about the given prototype of the given type.

## 3. `session`

### 3.1. `start`

```cmd
session start
```

#### **Description**

Starts a new session. Only one session can exist at a time.

### 3.2. `end`

```cmd
session end
```

#### **Description**

Ends current session.

## 4. `env`

### 4.1. `exec`

```cmd
env exec <code> [--file]
```

#### **Description**

Executes the given code/file.

**Options:**

4.1.1. `--file`

*Description*: If present `code` is treated as a path to the executed file.
*Default*: Inactive.

# Lua modding

When the application is loaded, it goes through the following steps:

1. Prototype loading step

Prototype loading step is when all general game data is loaded. This is also when `data.lua`, `data-updates.lua` and `data-final-fixes.lua` is executed.
To see deeper insight into this process, go to `lua-data.md`.

When a game is started, it goes through the following steps:

1. Starting session
2. Loading selected save

`control.lua` is executed right after step 1. However, there are also events related to the remaining steps. To see deeper insight into this process, go to `lua-control.md`.