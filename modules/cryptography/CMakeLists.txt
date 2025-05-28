if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|OpenBSD")
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		/usr/include/openssl/
	)
	find_library(SSL_LIB ssl)
	if (SSL_LIB)
		target_link_libraries(rex_module_${MODULE_NAME} ${SSL_LIB})
	endif()
	find_library(CRYPTO_LIB crypto)
	if (CRYPTO_LIB)
		target_link_libraries(rex_module_${MODULE_NAME} ${CRYPTO_LIB})
	endif()
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "FreeBSD")
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		/usr/local/include/openssl/
	)
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		C:/OpenSSL/include  # Generic location for Windows?
	)
	find_library(SSL_LIB libssl.lib)
	if (SSL_LIB)
		target_link_libraries(rex_module_${MODULE_NAME} ${SSL_LIB})
	endif()
	find_library(CRYPTO_LIB libcrypto.lib)
	if (CRYPTO_LIB)
		target_link_libraries(rex_module_${MODULE_NAME} ${CRYPTO_LIB})
	endif()
endif()

file(GLOB CRYPTO_SOURCES *.cpp)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${CRYPTO_SOURCES}
)
