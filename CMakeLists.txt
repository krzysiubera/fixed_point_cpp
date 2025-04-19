cmake_minimum_required(VERSION 3.10)
project(fixed-point-cpp CXX)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# enable debug symbols (like '-g') and all warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wconversion -Wpedantic -Wshadow -Werror")

# define the executable
set(SRC_FILES src/main.cpp)
add_executable(fixed-point-cpp ${SRC_FILES})

# link math library
target_link_libraries(fixed-point-cpp m)

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