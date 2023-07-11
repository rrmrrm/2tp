cmake_minimum_required(VERSION 3.14)
project(twoTp)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
#use google test
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(Main 
    codealg.h
    codealg.cpp
    main.cpp
)

add_executable(CodealgTest
    codealg.h
    codealg.cpp
    codealgTest.cpp
)
target_link_libraries(
    CodealgTest 
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(CodealgTest)