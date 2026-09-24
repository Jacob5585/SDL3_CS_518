cmake_minimum_required(VERSION 3.15)
project(hsv_project CXX C)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(hsv hsv.cpp color.c)

target_compile_options(hsv PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(image PRIVATE -g -fsanitize=address)
#target_link_options(image PRIVATE -fsanitize=address)

target_link_libraries(hsv PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)