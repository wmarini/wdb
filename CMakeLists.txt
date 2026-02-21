cmake_minimum_required(VERSION 3.30)

project(wdb
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "wdb is a native debugger"
)

# C++20 standard required for Boost.Cobalt
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cmake options
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libedit REQUIRED IMPORTED_TARGET libedit)

include(CTest)

add_subdirectory(src)
add_subdirectory(tools)

if (BUILD_TESTS)
    message(STATUS "Building tests")
    find_package(Catch2 CONFIG REQUIRED)
    enable_testing()
    add_subdirectory(test)
else()
    message(STATUS "Skipping tests")
endif()

if (BUILD_EXAMPLES)
    message(STATUS "Building examples")
    add_subdirectory(examples)
else()
    message(STATUS "Skipping examples")
endif()
    
