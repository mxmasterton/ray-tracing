cmake_minimum_required(VERSION 3.20)

project(ray-tracing LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ray-tracing main.cpp)

set_target_properties(ray-tracing PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
