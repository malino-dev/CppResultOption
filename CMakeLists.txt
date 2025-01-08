cmake_minimum_required(VERSION 3.28.3)
project(CppResultOption)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(tests)

add_library(CppResultOption INTERFACE)

target_include_directories(CppResultOption INTERFACE include/)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})