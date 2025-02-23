# sdl-subpixel

A test program to verify pixel-perfect positioning of textures and geometric
primitives (lines and points).


## How to build (Linux)

**Dependencies**: The project uses the [cmake](http://cmake.org) build system
and `SDL` 2.0. They can all be installed on a Debian-based
distribution using the following command:

    apt install cmake libsdl2-dev libsdl2-ttf-dev

**Build commands**:

    cmake -S. -Bhostbuild
    cd hostbuild
    make

## How to build (Wii)

**Dependencies**: the [devkitPro](https://devkitpro.org/) toolchain and the
SDL2 package installable from the devkitPro repositories.

**Build commands**:

    cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/cmake/Wii.cmake
    cd build
    make

If building for the GameCube, replace `Wii.cmake` with `Cube.cmake`.
