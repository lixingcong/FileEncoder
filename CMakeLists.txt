cmake_minimum_required(VERSION 3.5)

project(FileEncoder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(
	"include"
)

LINK_DIRECTORIES(
	"lib"
)

LINK_LIBRARIES(iconv)

add_executable(FileEncoder main.cpp)
