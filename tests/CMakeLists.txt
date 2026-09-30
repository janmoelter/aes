include(FetchContent)

set(NIST_CAVP_BASE_URL https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Algorithm-Validation-Program/documents/aes)

FetchContent_Declare(nist_cavp_kat URL ${NIST_CAVP_BASE_URL}/KAT_AES.zip URL_HASH SHA256=a203b16c9246b2ebae31dee5de21a606be80cf78ceabaca37150236fa098eb60 DOWNLOAD_EXTRACT_TIMESTAMP FALSE)
FetchContent_MakeAvailable(nist_cavp_kat)

add_executable(cavp_test cavp_test.cpp)
target_link_libraries(cavp_test PRIVATE AES::AES)


file(GLOB_RECURSE RSP_FILES CONFIGURE_DEPENDS ${nist_cavp_kat_SOURCE_DIR}/ECB*.rsp)

if(NOT RSP_FILES)
	message(FATAL_ERROR "No ECB response files found in the test vector archives")
endif()

foreach(RSP_FILE ${RSP_FILES})
	get_filename_component(TEST_NAME ${RSP_FILE} NAME_WE)
	
	add_test(NAME CAVP_AES_${TEST_NAME} COMMAND cavp_test ${RSP_FILE})
endforeach()
