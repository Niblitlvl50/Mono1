
cmake_minimum_required(VERSION 3.12.0)

project(mono1)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE Release)
endif()

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

# SDL
add_subdirectory("third_party/SDL")
include_directories(AFTER "${SDL_SOURCE_DIR}/include")

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

if(APPLE)
    find_library(AUDIOTOOLBOX AudioToolbox)
endif(APPLE)

# uuid4
add_library(uuid4 STATIC "third_party/uuid4/src/uuid4.c")

# delaunay
file(GLOB_RECURSE delaunay_source_files "third_party/delaunay/*.c")
add_library(delaunay STATIC ${delaunay_source_files})

# huffandpuff
file(GLOB_RECURSE huffandpuff_source_files "third_party/huffandpuff/*.c")
add_library(huffandpuff STATIC ${huffandpuff_source_files})

# par_streamlines
add_library(par_streamlines STATIC "third_party/par_streamlines/par_streamlines_impl.c")

# imgui
file(GLOB imgui_source_files "third_party/imgui/*.cpp")
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
add_dependencies(mono delaunay huffandpuff imgui chipmunk_static)
target_link_libraries(mono uuid4 delaunay huffandpuff par_streamlines imgui chipmunk_static SDL2-static OpenGL::GL ${AUDIOTOOLBOX})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${engine_source_files})


# Unit test
file(GLOB_RECURSE unittest_source_files "tests/*.cpp")
add_executable(unittest ${unittest_source_files})
add_dependencies(unittest mono gtest)
target_include_directories(unittest PRIVATE "third_party/gtest-1.7.0/include")
target_compile_definitions(unittest PRIVATE GTEST_HAS_TR1_TUPLE=0)
target_link_libraries(unittest mono gtest OpenGL::GL)
