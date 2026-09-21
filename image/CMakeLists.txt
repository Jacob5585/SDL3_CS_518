cmake_minimum_required(VERSION 3.15)
project(image_project CXX)

find_package(SDL3 CONFIG REQUIRED)

add_executable(image image.cpp)

target_compile_options(image PRIVATE -Wall -Wpedantic)

target_link_libraries(image PRIVATE SDL3::SDL3)