cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project("ZipInfoCpp")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX /MP)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory("src")