add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux|OpenBSD")
	target_include_directories(rex_engine INTERFACE
		/usr/include/openssl/
	)
	find_library(SSL_LIB ssl)
	if (SSL_LIB)
		list(APPEND BUILTIN_LIBRARIES ${SSL_LIB})
	endif()
	find_library(CRYPTO_LIB crypto)
	if (CRYPTO_LIB)
		list(APPEND BUILTIN_LIBRARIES ${CRYPTO_LIB})
	endif()
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "FreeBSD")
	target_include_directories(rex_engine INTERFACE
		/usr/local/include/openssl/
	)
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	target_include_directories(rex_engine INTERFACE
		C:/OpenSSL/include  # Generic location for Windows?
	)
	find_library(SSL_LIB libssl.lib)
	if (SSL_LIB)
		list(APPEND BUILTIN_LIBRARIES ${SSL_LIB})
	endif()
	find_library(CRYPTO_LIB libcrypto.lib)
	if (CRYPTO_LIB)
		list(APPEND BUILTIN_LIBRARIES ${CRYPTO_LIB})
	endif()
endif()

file(GLOB CRYPTO_SOURCES *.cpp)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${CRYPTO_SOURCES}
)

set(BUILTIN_LIBRARIES ${BUILTIN_LIBRARIES} PARENT_SCOPE)
