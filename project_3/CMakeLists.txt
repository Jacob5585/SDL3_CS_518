cmake_minimum_required(VERSION 3.15)
project(project_3 CXX C)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(project_3 project_3.cpp color.c)

target_compile_options(project_3 PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(image PRIVATE -g -fsanitize=address)
#target_link_options(image PRIVATE -fsanitize=address)

target_link_libraries(project_3 PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)