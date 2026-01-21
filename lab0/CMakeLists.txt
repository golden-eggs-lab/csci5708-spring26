cmake_minimum_required(VERSION 3.16)
project(lab0 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(lab0_core
        src/db.c
        src/csv.c
        src/operators.c
)

add_executable(lab0 src/main.c)
target_link_libraries(lab0 PRIVATE lab0_core)

add_executable(test_lab0 tests/test_lab0.c)
target_link_libraries(test_lab0 PRIVATE lab0_core)
