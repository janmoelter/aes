add_executable(examples examples.cpp)

target_link_libraries(examples
	PRIVATE AES::AES
)
