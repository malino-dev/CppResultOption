cmake_minimum_required(VERSION 3.28.3)
project(CppResultOption)
set(CMAKE_CXX_STANDARD 23)

find_package(GTest CONFIG REQUIRED)

add_executable(CppResultOption.Tests.Option tests/tests_option.cpp)
target_link_libraries(CppResultOption.Tests.Option GTest::gtest_main)
target_link_options(CppResultOption.Tests.Option PRIVATE -fsanitize=address)

#add_executable(FunctionalCpp.Tests.Result tests_result.cpp)
#target_link_libraries(FunctionalCpp.Tests.Result GTest::gtest_main)
#target_link_options(FunctionalCpp.Tests.Result PRIVATE -fsanitize=address)