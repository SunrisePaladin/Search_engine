cmake_minimum_required(VERSION 4.0)
project(Search_engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

# 1. Nlohmann JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# 2. GoogleTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads on)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

add_library(search_engine_core STATIC
        Sources/ConverterJSON.cpp
        Sources/InvertedIndex.cpp
        Sources/SearchServer.cpp
        Headers/SearchServer.h
        Headers/ConverterJSON.h
        Headers/InvertedIndex.h
        Tests/word_test.cpp
)

#add_subdirectory(nlohmann_json)
#include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
#include_directories(nlohmann_json/include)

target_include_directories(search_engine_core PUBLIC Headers)
target_link_libraries(search_engine_core PUBLIC nlohmann_json::nlohmann_json)

add_executable(Search_engine main.cpp)
target_link_libraries(Search_engine PRIVATE search_engine_core)

enable_testing()

add_executable(my_gtests
        Tests/inverted_index_test.cc
)

target_link_libraries(my_gtests
        PRIVATE
        search_engine_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(my_gtests)