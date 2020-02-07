cmake_minimum_required(VERSION 3.10)

# set the project name
project(mtfind)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(mtfind ${SOURCES})

target_link_libraries(mtfind PUBLIC ${CMAKE_THREAD_LIBS_INIT})
