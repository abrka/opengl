# OpenGL 3D Renderer

## Description 

This is an 3D renderer written in OpenGL


## Requirements

- Compiler that supports C++20
- CMake
- OpenGL Version 4.3

## Building

```shell
$ git clone https://github.com/abrka/opengl.git
$ cd opengl
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
Now run opengl_demo executable

### For Visual Studio
- Open opengl_proj.sln in Visual Studio
- Build and run opengl_demo


## How To Use
> **_NOTE:_** that the first time this program is run, the ui layout will be messed up. Drag and dock windows wherever you like to create your own layout. Next time the program is run, this layout will be used
### Controls
- WASD for flying
- Mouse to look around
- Left CTRL key for toggling cursor lock