cmake_minimum_required(VERSION 3.13)
project(SdlSubpixel VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(PROG sdl-subpixel)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL REQUIRED IMPORTED_TARGET sdl2)

add_executable(${PROG} src/main.c)
target_link_libraries(${PROG} PRIVATE PkgConfig::SDL -lm)
