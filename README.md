# hush-engine

A spatial-audio game engine designed for the creation of audio only games in C with support for 3D, spatialised audio. Uses SDL2, SDL_Sound and OpenAL. Offers a wrapping and extention of those libraries into a intuitive set of functions designed to aid audiogame development. Targets cross platform executables (although is currently designed with building on linux in mind).

## Installation
hush uses CMake to provide a library easily linked to your project with **most** inlcuded dependencies. 

### Unix installation guide:
First begin by cloning this git respository into your project folder, likley into the `/lib` or `/src` folder\. In order to include the attached git submodules, ensure you use the flag `--recursive-submodules`.\
`git clone --recursive-submodules "https://github.com/Ziggyh14/hush-engine" `\
Next, hush has one more required dependency in SDL2. Which can be installed with the command:\
`sudo apt-get install libsdl2-dev`\
Now hush is ready to be linked into your project. I'd recommend using CMake to build your executable and manage various dependencies (including hush).\
Heres an example of a very simple `CMakeLists.txt` file placed at the root of your project that will get you started.
```cmake
project(my_audiogame)

cmake_minimum_required(VERSION 3.16)

add_executable(out src/main.c)

add_subdirectory(src/hush-engine )

target_link_libraries(out PUBLIC hush::hush)
```
Once added, run `cmake build .` in your project's root directory then nativgate to the created `/build` directory and run `make` to build your executable. Which can be run now with `./build/out`

## Usage
Now what hush is linked, you can add
```C
#include "hush.h"
```
To access hush's features. 
Before doing anything, hush must be initialised using `hush_init()` at the start of your program and closed using `hush_init()` after it is no longer being used.\
To get started using hush, have a browse of `sample_func.h` which contains the meat of hush's playback features. 

## Beginners Guide
Coming soon...