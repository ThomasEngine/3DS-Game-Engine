cmake_minimum_required(VERSION 3.10)
project(3DSEngine)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D__3DS__ -DARM11)

file(GLOB_RECURSE ENGINE_SOURCES
        "engine/source/*.cpp"
        "engine/external/cjson/cJSON.c"
)

add_library(engine STATIC ${ENGINE_SOURCES})
target_include_directories(engine PUBLIC
        engine/include
        engine/include/systems
        engine/external/cjson
        $ENV{DEVKITPRO}/libctru/include
        $ENV{DEVKITPRO}/devkitARM/arm-none-eabi/include
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