cmake_minimum_required(VERSION 3.18)
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()
project(aes VERSION 1.0 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()


add_library(AES
	src/AES.cpp
	src/byte.cpp
)

add_library(AES::AES ALIAS AES)

target_include_directories(AES
	PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${PROJECT_SOURCE_DIR}/src
)

target_compile_features(AES PUBLIC cxx_std_14)

target_compile_options(AES
	PRIVATE
		-Wall -Wextra
		$<$<CONFIG:Debug>:-O0>
		$<$<CONFIG:Release>:-O3>
)


option(EXAMPLES "Build examples" OFF)

if(EXAMPLES)
	add_subdirectory(examples)
endif()
