cmake_minimum_required(VERSION 2.6)
project(compiler)

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")

set(SRC_DIR "src")

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Compiler.cpp
    ${SRC_DIR}/Conditions_Checker.cpp
    ${SRC_DIR}/Mode.cpp
    ${SRC_DIR}/Parser.cpp
    ${SRC_DIR}/Scanner.cpp
    ${SRC_DIR}/Code_Generator.cpp
)

include_directories(
    ${CMAKE_BINARY_DIR}/include
)

add_executable(compiler ${SOURCES}) 
