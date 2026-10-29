cmake_minimum_required(VERSION 3.15)
project(threading_project CXX C)
set(CMAKE_CXX_STANDARD 20)

# ON: fetches and builds in SDL3_gfx
# OFF: don't fetch or use SDL3_gfx
option(USE_SDL3_GFX "Choose whether to use SDL3_gfx" OFF)

if(USE_SDL3_GFX)
    include(FetchContent)
endif()


find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)


if(USE_SDL3_GFX)
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
endif()



add_executable(threading main.cpp framerate.cpp)

if(USE_SDL3_GFX)
    target_include_directories(threading PRIVATE ${sdl3_gfx_SOURCE_DIR})
endif()

target_compile_options(threading PRIVATE -Wall -Wpedantic)

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(threading PRIVATE -g -fsanitize=address)
#target_link_options(threading PRIVATE -fsanitize=address)

if(USE_SDL3_GFX)
    target_link_libraries(threading PRIVATE SDL3_gfx_Static)
endif()

target_link_libraries(threading PRIVATE
    SDL3_image::SDL3_image
#    SDL3_gfx_Static
    SDL3::SDL3
)
