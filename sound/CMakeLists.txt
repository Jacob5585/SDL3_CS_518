cmake_minimum_required(VERSION 3.15)
project(sound_project CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(sound sound.cpp framerate.cpp)

target_compile_options(sound PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(sound PRIVATE -g -fsanitize=address)
#target_link_options(sound PRIVATE -fsanitize=address)

target_link_libraries(sound PRIVATE
    SDL3_image::SDL3_image
    SDL3::SDL3
)