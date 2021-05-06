file(GLOB source_code "*.cpp")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_executable(ZipInfoCpp ${source_code})
    target_precompile_headers(ZipInfoCpp PRIVATE "PCH.hpp")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_executable(zip_info_cpp ${source_code})
    target_precompile_headers(ZipInfoCpp PRIVATE "PCH.hpp")
endif()