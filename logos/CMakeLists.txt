cmake_minimum_required(VERSION 3.15)
project(p5_conferw_project CXX)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

find_package(SDL3 CONFIG REQUIRED)
#find_package(SDL3_image CONFIG REQUIRED)


#---------------------------
# This if() is only here because of an issue in SDL3_gfx v1.0.1.
# Can be commented out if you also comment out the "GIT_TAG" part of the
# FetchContent_Declare() below to move to the most recent HEAD of SDL3_gfx.
if(TARGET SDL3::SDL3)
    set(SDL3_LIB SDL3::SDL3)
endif()


FetchContent_Declare(
    SDL3_gfx
    GIT_REPOSITORY https://github.com/sabdul-khabir/SDL3_gfx
    GIT_TAG v1.0.1
)
FetchContent_MakeAvailable(SDL3_gfx)



#---------------------------
# These two if() are only here because of an issue in SDL3_gfx.
# That project's CMakeLists.txt expects SDL3 headers to be located in a
# specific directory relative to the SDL3_gfx sources.  But since we are
# fetching SDL3_gfx at build time, that relative relationship can be broken.
# the GFX config does not leverage the modern find_package() feature of
# CMake, so these lines correct for that situation.
if(TARGET SDL3_gfx_Static)
    target_link_libraries(SDL3_gfx_Static PRIVATE SDL3::SDL3)
endif()
if(TARGET SDL3_gfx_Shared)
    target_link_libraries(SDL3_gfx_Shared PRIVATE SDL3::SDL3)
endif()



add_executable(p5_conferw logos.cpp)

target_include_directories(p5_conferw PRIVATE ${sdl3_gfx_SOURCE_DIR})
target_compile_options(p5_conferw PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(p5_conferw PRIVATE -g -fsanitize=address)
#target_link_options(p5_conferw PRIVATE -fsanitize=address)

target_link_libraries(p5_conferw PRIVATE
#    SDL3_image::SDL3_image
    SDL3_gfx_Static
    SDL3::SDL3
)