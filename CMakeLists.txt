cmake_minimum_required(VERSION 3.31)
project(DOD)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(Epidemix main.cpp
        include/benchmark.h
        src/entity.cpp
        include/entity.h)
target_link_libraries(Epidemix PRIVATE raylib)
target_include_directories(Epidemix PRIVATE include)