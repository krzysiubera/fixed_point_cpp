cmake_minimum_required(VERSION 3.10)
project(fixed-point-c C)

# enable debug symbols (like '-g') and all warnings
# TODO: -Wconversion -Wextra enable
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wpedantic -Wshadow -Werror")

# gather source files
file(GLOB SRC_FILES src/*.c)

# define the executable
add_executable(fixed-point-c ${SRC_FILES})

# link math library
target_link_libraries(fixed-point-c m)

# custom 'run' target
add_custom_target(run 
    COMMAND ${CMAKE_BINARY_DIR}/build/fixed-point-c
    DEPENDS fixed-point-c
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/build
)

# optional: custom clean to delete build directory content
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/CMakeCache.txt
)