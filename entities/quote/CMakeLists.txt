cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

add_library(quote "quote.cpp")

include(../../cmake/gtest.cmake)
INIT_TEST(quote)
