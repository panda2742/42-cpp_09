cmake_minimum_required(VERSION 3.10)
project(Visualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -MD -Wshadow -g3 --std=c++23")

include_directories(include)

set(SOURCES
	src/main.cpp
)

add_executable(Visualizer ${SOURCES})
