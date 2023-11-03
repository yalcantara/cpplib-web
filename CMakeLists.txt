cmake_minimum_required(VERSION 3.10)
project(cpplib-web C CXX)

add_executable(web-main web-main.cpp)
add_executable(web-test web-test.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wno-multichar")

include_directories(include)
include_directories(../cpplib-core/include)

target_include_directories(web-test PUBLIC test-include)

find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL 1.0.0 REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(CURL REQUIRED)

target_link_libraries(web-main
        CURL::libcurl
        cpprestsdk::cpprest
        Boost::boost
        Boost::system
        OpenSSL::SSL
        ${CMAKE_DL_LIBS})

target_link_libraries(web-test CURL::libcurl)