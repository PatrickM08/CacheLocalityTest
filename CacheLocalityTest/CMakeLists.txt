cmake_minimum_required(VERSION 3.15)
project(CacheLocalityTest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CacheLocalityTest main.cpp)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    target_compile_options(CacheLocalityTest PRIVATE
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
    )
else()
    target_compile_options(CacheLocalityTest PRIVATE
        $<$<CONFIG:Release>:-O3 -DNDEBUG>
    )
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
