cmake_minimum_required(VERSION 3.31)
project(DOD LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
find_package(OpenMP REQUIRED)

include(FetchContent)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(Epidemix main.cpp
        oop/include/benchmark.h
        oop/src/benchmark.cpp
        oop/src/entity.cpp
        oop/include/entity.h
        oop/src/main_oop.cpp
        oop/include/main_oop.h
        dod/src/main_dod.c
        dod/include/main_dod.h
        dod/include/benchmark.h
        dod/src/benchmark.c
        dod/include/entity.h
        dod/src/entity.c
        config.h
        config.c)

target_link_libraries(Epidemix
        PRIVATE
        raylib
        OpenMP::OpenMP_C
        OpenMP::OpenMP_CXX
)

target_link_options(Epidemix PRIVATE -fopenmp)

target_include_directories(Epidemix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
