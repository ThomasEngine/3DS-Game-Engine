cmake_minimum_required(VERSION 3.10)
project(3DSEngine)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__3DS__ -DARM11)

# IDE indexing only — the real build is the Makefile. The DEVKITPRO env var
# can't be used directly: on Windows the installer sets it to /opt/devkitpro
# (an MSYS2 path native CMake can't resolve), and IDEs launched outside a
# shell may not see it at all. Fall back to the platform's install location.
set(DEVKITPRO "$ENV{DEVKITPRO}")
if(NOT EXISTS "${DEVKITPRO}/libctru/include")
    if(WIN32)
        set(DEVKITPRO "C:/devkitPro")
    else()
        set(DEVKITPRO "/opt/devkitpro")
    endif()
endif()

file(GLOB_RECURSE ENGINE_SOURCES
        "engine/source/*.cpp"
        "engine/external/cjson/cJSON.c"
)

add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC
        engine/include
        engine/include/systems
        engine/external/cjson
        ${DEVKITPRO}/libctru/include
        ${DEVKITPRO}/devkitARM/arm-none-eabi/include
)

file(GLOB GAME_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/games ${CMAKE_SOURCE_DIR}/games/*)

foreach(game ${GAME_DIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/games/${game})
        file(GLOB_RECURSE GAME_SOURCES "games/${game}/source/*.cpp")

        if(GAME_SOURCES)
            add_executable(${game} ${GAME_SOURCES})
            target_include_directories(${game} PRIVATE games/${game}/include)
            target_link_libraries(${game} engine)
            message(STATUS "Created target: ${game}")
        endif()
    endif()
endforeach()