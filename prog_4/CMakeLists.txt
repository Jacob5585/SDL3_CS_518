cmake_minimum_required(VERSION 3.15)
project(prog_4_project CXX C)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(generator generator.cpp)
set_property(TARGET generator PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/temp")

set(ASSET_OUTPUT_HEADER "${CMAKE_CURRENT_BINARY_DIR}/assets.h")
set(ASSET_OUTPUT_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/assets.cpp")

set(ASSET_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/backdrop.jpeg"
    "${CMAKE_CURRENT_SOURCE_DIR}/pizza.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/pusheen.png"
)

# Define the command to run the generator
add_custom_command(
    OUTPUT ${ASSET_OUTPUT_HEADER} ${ASSET_OUTPUT_SOURCE}
    COMMAND $<TARGET_FILE:generator> ${ASSET_FILES}
    DEPENDS generator ${ASSET_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(prog_4 prog_4.cpp framerate.cpp ${ASSET_OUTPUT_SOURCE})

target_compile_options(prog_4 PRIVATE -Wall -Wpedantic)
target_include_directories(prog_4 PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

### Uncomment these two lines on Linux for address sanitizer.
### This helps you identify the sources of segmentation faults.
### May not work outside Linux.

#target_compile_options(prog_4 PRIVATE -g -fsanitize=address)
#target_link_options(prog_4 PRIVATE -fsanitize=address)

target_link_libraries(prog_4 PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)