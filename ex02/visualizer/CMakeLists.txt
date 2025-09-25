cmake_minimum_required(VERSION 3.10)
project(Visualizer LANGUAGES CXX)

include(FetchContent)
 
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
	GIT_TAG v6.1.9
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -MD -Wshadow -g3 --std=c++23")

add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fexperimental-library>)

include_directories(include)

set(SOURCES
	src/main.cpp
	src/Utils.cpp
	src/Visualizer.cpp
)

add_executable(Visualizer ${SOURCES})

target_link_libraries(Visualizer
	PRIVATE ftxui::screen
	PRIVATE ftxui::dom
	PRIVATE ftxui::component
)

target_include_directories(Visualizer PUBLIC
	${CMAKE_SOURCE_DIR}/include
	${ftxui_SOURCE_DIR}/include
)
