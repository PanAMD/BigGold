cmake_minimum_required(VERSION 3.28)
project(BigGold)

set(CMAKE_CXX_STANDARD 23)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Now find_package should work
add_executable(BigGold main.cpp
        BigMacDAO/bigmacdao.cpp
        BigMacDAO/bigmacdao.h
        PythonManager/pythonmanager.h)

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/Resources")
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/Resources")

include(FetchContent)
include(ExternalProject)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTING OFF)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8
        CMAKE_ARGS -DCPR_BUILD_TESTS=OFF) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-18-0
)
FetchContent_MakeAvailable(asio)

add_library(asio INTERFACE)


if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Building on Windows")

    execute_process(
            COMMAND ${Python3_EXECUTABLE} -m pybind11 --includes
            OUTPUT_VARIABLE PYBIND11_INCLUDES
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    # Remove the '-I' prefix and replace '-I' with ';' to split into a list
    # Remove the '-I' prefix and replace '-I' with ';' to split into a list
    string(REPLACE "-I" ";" PYBIND11_INCLUDE_DIRS "${PYBIND11_INCLUDES}")

    # Remove leading semicolon, if any
    string(REGEX REPLACE "^;" "" PYBIND11_INCLUDE_DIRS "${PYBIND11_INCLUDE_DIRS}")

    # Now, we need to filter the include paths to keep only the one containing "pybind11"
    foreach(INCLUDE_DIR ${PYBIND11_INCLUDE_DIRS})
        if(INCLUDE_DIR MATCHES ".*pybind11.*")
            set(PYBIND11_INCLUDE_DIR ${INCLUDE_DIR})
        endif()
    endforeach()

    target_include_directories(BigGold PUBLIC ${PYBIND11_INCLUDE_DIR})

else()
    message(STATUS "Building on an UNIX-BASED OS: ${CMAKE_SYSTEM_NAME}")
    find_package(pybind11 REQUIRED)

endif()

# Create a custom command to copy resources
add_custom_command(
        TARGET BigGold POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCE_DIR} ${DESTINATION_DIR}
        COMMENT "Copying resource files to the output directory"
)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)


target_link_libraries(BigGold PRIVATE Python3::Python cpr::cpr asio)
