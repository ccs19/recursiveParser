cmake_minimum_required(VERSION 2.8.4)
project(recusriveParser)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

set(SOURCE_FILES Lex.c SymbolTable.c)
add_executable(recusriveParser ${SOURCE_FILES})