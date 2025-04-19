cmake_minimum_required(VERSION 3.10)
project(fixed-point-cpp CXX)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add the executable target
add_executable(fixed-point-cpp src/test_fixed_point.cpp)

# enable debug symbols (like '-g') and all warnings
target_compile_options(fixed-point-cpp PRIVATE -g -Wall -Wextra -Wconversion -Wpedantic -Wshadow -Werror)

# include directory for fixed point math lib
target_include_directories(fixed-point-cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/fixed_point)

# custom 'run' target
add_custom_target(run 
    COMMAND ${CMAKE_BINARY_DIR}/build/fixed-point-cpp
    DEPENDS fixed-point-cpp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

# optional: custom clean to delete build directory content
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
)