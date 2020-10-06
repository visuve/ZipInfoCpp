cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
FILE(GLOB source_code *.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_executable (ZipInfoCpp ${source_code})
    target_precompile_headers(ZipInfoCpp PRIVATE "PCH.hpp")
    target_compile_options(ZipInfoCpp PRIVATE /W4 /WX)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable (zip_info_cpp ${source_code})
    target_precompile_headers(ZipInfoCpp PRIVATE "PCH.hpp")
    target_compile_options(zip_info_cpp PRIVATE -Wall -Wextra -pedantic -Werror)
endif()