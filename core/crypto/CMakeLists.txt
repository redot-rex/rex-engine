
# TODO: Checking for Module/Built-in logic isn't implemented
if (BUILTIN_MBEDTLS)
	target_include_directories(rex_core PUBLIC
		${CMAKE_SOURCE_DIR}/thirdparty/mbedtls/include/
	)
endif()

file(GLOB_RECURSE CORE_CRYPTO_SOURCES *.cpp)
target_sources(rex_core PUBLIC ${CORE_CRYPTO_SOURCES})
