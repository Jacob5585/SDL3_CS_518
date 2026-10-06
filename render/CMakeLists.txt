cmake_minimum_required(VERSION 3.15)
project(render_project CXX C)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(render render.cpp framerate.cpp)

target_compile_options(render PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(render PRIVATE -g -fsanitize=address)
#target_link_options(render PRIVATE -fsanitize=address)

target_link_libraries(render PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)