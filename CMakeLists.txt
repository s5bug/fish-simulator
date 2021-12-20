cmake_minimum_required(VERSION 3.21)
project(fish-simulator)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib 4.0 QUIET)

if (NOT raylib_FOUND)
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()

endif()

add_executable(fish-simulator src/main.cpp src/fish.h src/entity.h src/world.h src/player_fish.h src/ai_fish.h src/ai_fish.cpp src/player_fish.cpp src/entity.cpp src/fish.cpp src/entity_quad_map.cpp src/entity_quad_map.h src/world.cpp)
target_link_libraries(fish-simulator raylib)
