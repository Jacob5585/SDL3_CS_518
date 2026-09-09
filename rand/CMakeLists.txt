cmake_minimum_required(VERSION 3.15)
project(rand_project CXX)

find_package(SDL3 CONFIG REQUIRED)

add_executable(rand rand.cpp)

target_compile_options(rand PRIVATE -Wall -Wpedantic)

target_link_libraries(rand PRIVATE SDL3::SDL3)