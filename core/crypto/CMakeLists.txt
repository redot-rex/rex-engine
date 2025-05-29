
# TODO: Checking for Module/Built-in logic isn't implemented
if (BUILTIN_MBEDTLS)
	target_include_directories(rex_core PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/mbedtls/include/
	)
endif()

file(GLOB_RECURSE CORE_CRYPTO_SOURCES *.cpp)
target_sources(rex_core PRIVATE ${CORE_CRYPTO_SOURCES})
