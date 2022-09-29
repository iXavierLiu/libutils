cmake_minimum_required(VERSION 3.10)

project(libutils-tests LANGUAGES CXX)

if(BDUIL_UNITEST)
	find_package(GTest CONFIG REQUIRED)

	set(_TEST_NAME ${PROJECT_NAME}-unit)

	file(GLOB _SRC "./Test*.cpp")
	add_executable(${_TEST_NAME} ${_SRC})
	
	target_link_libraries(${_TEST_NAME} PRIVATE libutils::libutils_all GTest::gtest_main)
	target_compile_options(${_TEST_NAME} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()



if(BDUIL_TEST)
	set(_TEST_NAME ${PROJECT_NAME}-global)

	add_executable(${_TEST_NAME} "main.cpp")
	target_link_libraries(${_TEST_NAME} PRIVATE libutils::libutils_all)
	target_compile_options(${_TEST_NAME} PRIVATE "$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

