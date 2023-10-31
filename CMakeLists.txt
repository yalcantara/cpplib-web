cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Project
project(main)
add_executable(main main.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wno-multichar")

include_directories(include)
include_directories(../cpplib-core/include)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL 1.0.0 REQUIRED)
find_package(cpprestsdk REQUIRED)

target_link_libraries(main
        cpprestsdk::cpprest
        Boost::boost
        Boost::system
        OpenSSL::SSL
        ${CMAKE_DL_LIBS})