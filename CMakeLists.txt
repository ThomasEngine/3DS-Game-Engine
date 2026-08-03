cmake_minimum_required(VERSION 3.10)
project(3DSEngine)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__3DS__ -DARM11)

# --- Engine as a static library ---
file(GLOB_RECURSE ENGINE_SOURCES
        "engine/source/*.cpp"
        "engine/external/cjson/cJSON.c"
)

add_library(engine STATIC ${ENGINE_SOURCES} "engine/source/network_system.cpp" "engine/include/systems/network_system.h")
target_include_directories(engine PUBLIC
        engine/include
        engine/include/systems
        engine/external/cjson
        C:/devkitPro/libctru/include
        C:/devkitPro/devkitARM/arm-none-eabi/include
)

# --- Auto-detect and create a target per game ---
file(GLOB GAME_DIRS RELATIVE ${CMAKE_SOURCE_DIR}/games ${CMAKE_SOURCE_DIR}/games/*)

foreach(game ${GAME_DIRS})
    if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/games/${game})
        file(GLOB_RECURSE GAME_SOURCES "games/${game}/source/*.cpp")

        # only create a target if the game has source files
        if(GAME_SOURCES)
            add_executable(${game} ${GAME_SOURCES} "engine/source/network_system.cpp" "engine/include/systems/network_system.h")
            target_include_directories(${game} PRIVATE games/${game}/include)
            target_link_libraries(${game} engine)
            message(STATUS "Created target: ${game}")
        endif()
    endif()
endforeach()