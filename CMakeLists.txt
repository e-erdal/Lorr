cmake_minimum_required(VERSION 3.20)

include(CMake/LorrComponent.cmake)

project(Lorr VERSION 0.1.0 LANGUAGES CXX C)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DNOMINMAX)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

option(ENGINE_TRACING_ENABLED "Enable Tracing" OFF)
if (ENGINE_TRACING_ENABLED)
    message("Tracy is enabled!")
    add_definitions(-DTRACY_ENABLE) # Enable tracing solution wide.
endif()

add_subdirectory(Vendor)
add_subdirectory(Lorr)