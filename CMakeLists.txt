cmake_minimum_required(VERSION 3.5)

project(apiderg LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)


add_executable(apiderg main.cpp webclient.h webclient.cpp json.h json.cpp)

target_link_libraries(apiderg OpenSSL::SSL)

