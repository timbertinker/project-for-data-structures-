cmake_minimum_required(VERSION 3.3)
project(SDChecker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SOURCE_FILES main.cpp IOUtils.h CheckUtils.h Constants.h)
add_executable(SDChecker ${SOURCE_FILES})