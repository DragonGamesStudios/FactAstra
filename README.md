 # FactAstra
 
## About

FactAstra project is a game, where you build factories in space. Collect resources from planets, moons and asteroids, deliver them to your orbital stations and go forward in technology to finally build an Einstein - Rosen drive and go home.

The game is designed following the rule of "if something is possible through GUI, it should also be possible through command line". And so, the game can be controlled with command line. All the commands and their usage is described in `help.md`.

The project is free software in both meanings. Feel free to use any portion of the code as long as you include the copy of the license in your project (I suppose orbital pathfinder might be useful in some way, though it's not yet creted).

## Installation

To install FactAstra, follow these steps:

1. Clone the repository

```
git clone "https://github.com/DragonGamesStudios/FactAstra.git"
```

2. Build with CMake

```
cmake -B ./build -G <your generator> .
```

3. Build with your compiler.

## Dependencies
FactAstra project has the following dependencies:
* [Spectre2D](https://github.com/DragonGamesStudios/Spectre2D)
* [zlib](http://www.zlib.net/)

Spectre2D library also has its dependencies. Since it's static, they also need to be included in the build.

Note, that the project is tested on Windows/Linux. Thus, if there are compilation/build errors, first check if there are all library files in the right directory (`extlibs/<System>/<Configuration>`). If not, you'll probably need to build the libraries from source.

 ## Usage

 Now all you have to do is simply run the executable. However, if you just want to make sure everything's fine, simply open cmd and write:

 ```
FactAstra --console
 ```

 And then, when `>>>` shows up, simply write

 ```
quit
 ```

 If the program terminates with no errors, everything should be fine. For any help with the commands, check out `help.md`.