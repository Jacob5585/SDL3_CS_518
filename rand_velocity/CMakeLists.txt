cmake_minimum_required(VERSION 3.15)
project(rand_project CXX)

find_package(SDL3 CONFIG REQUIRED)

add_executable(rand_velocity rand_velocity.cpp)

target_compile_options(rand_velocity PRIVATE -Wall -Wpedantic)

target_link_libraries(rand_velocity PRIVATE SDL3::SDL3)