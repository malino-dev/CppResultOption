find_package(GTest CONFIG REQUIRED)

add_executable(FunctionalCpp.Tests.Option tests_option.cpp)
target_link_libraries(FunctionalCpp.Tests.Option GTest::gtest_main)
target_link_options(FunctionalCpp.Tests.Option PRIVATE -fsanitize=address)

#add_executable(FunctionalCpp.Tests.Result tests_result.cpp)
#target_link_libraries(FunctionalCpp.Tests.Result GTest::gtest_main)
#target_link_options(FunctionalCpp.Tests.Result PRIVATE -fsanitize=address)