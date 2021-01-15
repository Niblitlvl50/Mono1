
cmake_minimum_required(VERSION 3.12.0)

project(mono1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

if(MSVC)
    # 4100 "unreferenced formal parameter"
    # 4244 "conversion from '__int64' to 'uint32_t', possible loss of data"
    # 4245 "conversion from 'int' to 'uint32_t', signed/unsigned mismatch"
    # 4267 "conversion from 'size_t' to 'uint32_t', possible loss of data"
    # 4505 "unreferenced local function has been removed"
    add_compile_options("/W4" "/wd4100" "/wd4244" "/wd4245" "/wd4267" "/wd4505" "$<$<CONFIG:RELEASE>:/O2>")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS=1)
else()
    add_compile_options("-Wall" "-Wextra" "-Wno-unused-parameter" "$<$<CONFIG:RELEASE>:-O3>")
endif()

# Chipmunk2D
set(BUILD_SHARED OFF CACHE BOOL "Build and install the shared library" FORCE)
set(BUILD_STATIC ON)
set(BUILD_DEMOS OFF CACHE BOOL "Build chipmunk demo application" FORCE)
set(LIB_INSTALL_DIR ${LIBRARY_OUTPUT_PATH})
add_subdirectory("third_party/Chipmunk2D")

include_directories("src")
include_directories(SYSTEM "third_party")
include_directories(SYSTEM ${chipmunk_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)

find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
endif(SDL2_FOUND)

# huffandpuff
file(GLOB_RECURSE huffandpuff_source_files "third_party/huffandpuff/*.c" "third_party/huffandpuff/*.h")
add_library(huffandpuff STATIC ${huffandpuff_source_files})

# imgui
file(GLOB_RECURSE imgui_source_files "third_party/imgui/*.cpp" "third_party/imgui/*.h")
add_library(imgui STATIC ${imgui_source_files})

# gtest
add_library(gtest STATIC "third_party/gtest-1.7.0/src/gtest-all.cc")
target_include_directories(gtest PRIVATE "third_party/gtest-1.7.0/include" "third_party/gtest-1.7.0")
target_compile_definitions(gtest PRIVATE GTEST_HAS_TR1_TUPLE=0)
if(NOT MSVC)
    target_compile_options(gtest PRIVATE -Wno-missing-field-initializers)
endif()

# Mono
file(GLOB_RECURSE engine_source_files "src/*.cpp" "src/*.h")
add_library(mono STATIC ${engine_source_files})
add_dependencies(mono huffandpuff imgui chipmunk_static)
target_link_libraries(mono huffandpuff imgui chipmunk_static ${SDL2_LIBRARY} ${OPENGL_LIBRARIES})

# Unit test
file(GLOB_RECURSE unittest_source_files "tests/*.cpp")
add_executable(unittest ${unittest_source_files})
add_dependencies(unittest mono gtest)
target_include_directories(unittest PRIVATE "third_party/gtest-1.7.0/include")
target_compile_definitions(unittest PRIVATE GTEST_HAS_TR1_TUPLE=0)
target_link_libraries(unittest mono gtest ${OPENGL_LIBRARIES})
