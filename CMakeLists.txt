cmake_minimum_required(VERSION 3.14)
project(dumptermios)

set(CMAKE_CXX_STANDARD 14)

add_executable(dumptermios main.cpp termiosutils.cpp termiosutils.h)
