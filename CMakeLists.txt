cmake_minimum_required(VERSION 3.28.3)
project(CppResultOption)
set(CMAKE_CXX_STANDARD 23)

add_subdirectory(tests)

add_library(CppResultOption INTERFACE)

target_include_directories(CppResultOption INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS CppResultOption
        EXPORT CppResultOptionTargets)

install(DIRECTORY include/ DESTINATION include)