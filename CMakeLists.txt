cmake_minimum_required(VERSION 3.8)

project(mymemory)

add_executable(mymemory
    src/main.cpp
)

target_include_directories(mymemory PRIVATE ${CMAKE_SOURCE_DIR}/include)