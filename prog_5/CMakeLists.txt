cmake_minimum_required(VERSION 3.15)
project(p5_project CXX C)

find_package(SDL3 CONFIG REQUIRED)
# find_package(SDL3_gfx CONFIG REQUIRED)
find_library(SDL3_GFX_LIBRARY NAMES SDL3_gfx)

add_executable(p5 p5.cpp)

target_compile_options(p5 PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(p5 PRIVATE -g -fsanitize=address)
#target_link_options(p5 PRIVATE -fsanitize=address)

target_link_libraries(p5 PRIVATE
    SDL3::SDL3
    # SDL3_gfx::SDL3_gfx
    ${SDL3_GFX_LIBRARY}
)