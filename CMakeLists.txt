cmake_minimum_required(VERSION 3.29)
project(parsers)

set(CMAKE_CXX_STANDARD 14)

add_executable(parsers main.cpp
        grammar/grammar.cpp
        encoded_rules/encoded_rules.cpp
        encoded_rules/encoded_rules.h)
