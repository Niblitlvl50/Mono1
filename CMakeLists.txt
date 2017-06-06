
cmake_minimum_required(VERSION 2.6)

project(mono1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wno-long-long -pedantic -Wno-unused-parameter -Wno-unused-variable -Wno-format-pedantic")

# Chipmunk2D
set(BUILD_SHARED OFF)
set(BUILD_STATIC ON)
set(BUILD_DEMOS OFF CACHE BOOL "Build chipmunk demo application" FORCE)
set(LIB_INSTALL_DIR ${LIBRARY_OUTPUT_PATH})
add_subdirectory("External Libs/Chipmunk2D")


include_directories("External Libs" "MonoEngine" "Shared")
include_directories(${chipmunk_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)

find_package(OpenAL REQUIRED)
if(OPENAL_FOUND)
    include_directories(${OPENAL_INCLUDE_DIR})
endif(OPENAL_FOUND)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
endif(SDL2_FOUND)

# imgui
file(GLOB_RECURSE imgui_source_files "External Libs/imgui/*.cpp")
add_library(imgui STATIC ${imgui_source_files})

# gtest
file(GLOB_RECURSE gtest_source_files "External Libs/gtest-1.7.0/src/*.cc")
add_library(gtest STATIC ${gtest_source_files})
target_include_directories(gtest PRIVATE "External Libs/gtest-1.7.0/include" "External Libs/gtest-1.7.0")

# Shared code
# file(GLOB_RECURSE shared_source_files "Shared/*.cpp")
# add_library(shared STATIC ${shared_source_files})

# Mono
file(GLOB_RECURSE engine_source_files "MonoEngine/*.cpp")
add_library(mono STATIC ${engine_source_files})
add_dependencies(mono imgui chipmunk_static)
target_link_libraries(mono imgui chipmunk_static ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${OPENAL_LIBRARY})

# Unit test
file(GLOB_RECURSE unittest_source_files "UnitTests/*.cpp")
add_executable(unittest ${unittest_source_files})
add_dependencies(unittest mono gtest)
target_include_directories(unittest PRIVATE "External Libs/gtest-1.7.0/include")
target_link_libraries(unittest mono gtest)

# Editor
# file(GLOB_RECURSE editor_source_files "Editor/*.cpp")
# add_executable(editor ${editor_source_files})
# add_dependencies(editor mono shared imgui)
# target_include_directories(editor PRIVATE "Editor")
# target_link_libraries(editor mono shared imgui)

# Game
# file(GLOB_RECURSE game_source_files "Game/*.cpp")
# add_executable(game ${game_source_files})
# add_dependencies(game mono shared)
# target_include_directories(game PRIVATE "Game")
# target_link_libraries(game mono shared)

