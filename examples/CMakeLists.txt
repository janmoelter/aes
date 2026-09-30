add_executable(example_vectors example_vectors.cpp)

target_link_libraries(example_vectors
	PRIVATE AES::AES
)
