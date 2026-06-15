cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_PROJECT_NAME embedded_patterns)

include_directories(inc)

add_library(core_logic STATIC 
    src/ring_buffer.c
)
add_library(tests STATIC 
    tests/test_ring_buffer.c
)

add_executable(embedded_patterns tests/main.c)
target_link_libraries(embedded_patterns core_logic)

target_link_libraries(tests core_logic)
target_link_libraries(embedded_patterns tests)