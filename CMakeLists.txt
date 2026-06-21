cmake_minimum_required(VERSION 3.5)
project(Bomberman3DS)

set(CMAKE_CXX_STANDARD 17)

add_definitions(
        -D__3DS__
        -DARM11
)

include_directories(
        engine/include
        game/include
        C:/devkitPro/libctru/include
        C:/devkitPro/devkitARM/arm-none-eabi/include
        C:/devkitPro/devkitARM/arm-none-eabi/include/c++/15.2.0
        C:/devkitPro/devkitARM/arm-none-eabi/include/c++/15.2.0/arm-none-eabi
        C:/devkitPro/devkitARM/lib/gcc/arm-none-eabi/15.2.0/include
)

# Glob everything recursively
file(GLOB_RECURSE SOURCES
        "engine/source/*.cpp"
        "engine/include/*.h"
        "game/source/*.cpp"
        "game/include/*.h"
)

add_executable(bomberman ${SOURCES})