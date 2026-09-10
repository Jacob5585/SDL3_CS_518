cmake_minimum_required(VERSION 3.15)
project(sliding_color_project CXX)

find_package(SDL3 CONFIG REQUIRED)

add_executable(main main.cpp)

target_compile_options(main PRIVATE -Wall -Wpedantic)

target_link_libraries(main PRIVATE SDL3::SDL3)