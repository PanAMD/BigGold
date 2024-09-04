cmake_minimum_required(VERSION 3.28)
project(BigGold)

set(CMAKE_CXX_STANDARD 20)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Now find_package should work
find_package(pybind11 REQUIRED)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Building on Windows")
else()
    message(STATUS "Building on an UNIX-BASED OS: ${CMAKE_SYSTEM_NAME}")
endif()

add_executable(BigGold main.cpp
        server.cpp
        server.h)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/Resources")

# Create a custom command to copy resources
add_custom_command(
        TARGET BigGold POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} ${DESTINATION_DIR}
        COMMENT "Copying resource files to the output directory"
)

target_link_libraries(BigGold PRIVATE pybind11::embed Python3::Python curl)