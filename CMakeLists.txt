cmake_minimum_required(VERSION 3.20)

project(ray-tracing LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG a7d4c2175630e1ea5b6f8dd00ab96fed0b97b96d
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(stb)

find_package(Threads REQUIRED)

add_executable(ray-tracing
    image_writer.cpp
    main.cpp
)
target_include_directories(ray-tracing PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(ray-tracing PRIVATE Threads::Threads)

set_target_properties(ray-tracing PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
