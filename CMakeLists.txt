cmake_minimum_required(VERSION 3.20)
project(CPPDVD LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CPPDVD old.cpp
        andray.cpp
        andray.h
)

include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)
target_link_libraries(CPPDVD PRIVATE raylib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if (MINGW)
    message(STATUS "MinGW detected → enabling static linking")

    target_link_options(CPPDVD PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
    )
endif()

if (APPLE)
    target_link_libraries(CPPDVD PRIVATE
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreFoundation"
    )
endif()