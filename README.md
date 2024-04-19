# hush-engine

A spatial-audio game engine designed for the creation of audio only games in C with support for 3D, spatialised audio. Uses SDL2, SDL_Sound and OpenAL. Offers a wrapping and extention of those libraries into a intuitive set of functions designed to aid audiogame development. Targets cross platform executables (although is currently designed with building on linux in mind).

## Installation
hush uses a Cmake to provide a library easily linked to your project with **most** inlcuded dependencies. 

### Unix installation guide:
First begin by cloning this git respository into your project folder, likley into the `/lib` or `/src` folders. In order to include the attached git submodules, ensure you use the flag `--recursive-submodules`.
`git clone --recursive-submodules "https://github.com/Ziggyh14/hush-engine" `
