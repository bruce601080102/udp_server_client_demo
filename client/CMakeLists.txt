cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUID_TYPE  Debug)
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS } -std=c++14")

project(sample)

add_executable(client client.cpp ${allcpp})

