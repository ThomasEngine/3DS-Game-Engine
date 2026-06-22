cmake_minimum_required(VERSION 3.5)
project(Bomberman3DS)

set(CMAKE_CXX_STANDARD 17)

add_definitions(
        -D__3DS__
        -DARM11
)

include_directories(
        engine/include
        engine/include/systems
        games/bomberman/include
        games/platformer/include
        games/topdown/include
        games/blank/include
        C:/devkitPro/libctru/include
        C:/devkitPro/devkitARM/arm-none-eabi/include
)

# Glob everything recursively
file(GLOB_RECURSE SOURCES
        "engine/source/*.cpp"
        "engine/include/*.h"
        "games/*/source/*.cpp"
        "games/*/include/*.h"
)

add_executable(bomberman ${SOURCES})