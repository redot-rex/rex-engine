add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
if (BUILTIN_MBEDTLS)
	set(THIRDPARTY_MBEDTLS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/mbedtls/)
	add_library(mbedtls OBJECT
        ${THIRDPARTY_MBEDTLS_DIR}library/aes.c
        ${THIRDPARTY_MBEDTLS_DIR}library/aesce.c
        ${THIRDPARTY_MBEDTLS_DIR}library/aesni.c
        ${THIRDPARTY_MBEDTLS_DIR}library/aria.c
        ${THIRDPARTY_MBEDTLS_DIR}library/asn1parse.c
        ${THIRDPARTY_MBEDTLS_DIR}library/asn1write.c
        ${THIRDPARTY_MBEDTLS_DIR}library/base64.c
        ${THIRDPARTY_MBEDTLS_DIR}library/bignum.c
        ${THIRDPARTY_MBEDTLS_DIR}library/bignum_core.c
        ${THIRDPARTY_MBEDTLS_DIR}library/bignum_mod_raw.c
        ${THIRDPARTY_MBEDTLS_DIR}library/camellia.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ccm.c
        ${THIRDPARTY_MBEDTLS_DIR}library/chacha20.c
        ${THIRDPARTY_MBEDTLS_DIR}library/chachapoly.c
        ${THIRDPARTY_MBEDTLS_DIR}library/cipher.c
        ${THIRDPARTY_MBEDTLS_DIR}library/cipher_wrap.c
        ${THIRDPARTY_MBEDTLS_DIR}library/cmac.c
        ${THIRDPARTY_MBEDTLS_DIR}library/constant_time.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ctr_drbg.c
        ${THIRDPARTY_MBEDTLS_DIR}library/debug.c
        ${THIRDPARTY_MBEDTLS_DIR}library/des.c
        ${THIRDPARTY_MBEDTLS_DIR}library/dhm.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ecdh.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ecdsa.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ecjpake.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ecp.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ecp_curves.c
        ${THIRDPARTY_MBEDTLS_DIR}library/entropy.c
        ${THIRDPARTY_MBEDTLS_DIR}library/entropy_poll.c
        ${THIRDPARTY_MBEDTLS_DIR}library/error.c
        ${THIRDPARTY_MBEDTLS_DIR}library/gcm.c
        ${THIRDPARTY_MBEDTLS_DIR}library/hkdf.c
        ${THIRDPARTY_MBEDTLS_DIR}library/hmac_drbg.c
        ${THIRDPARTY_MBEDTLS_DIR}library/md.c
        ${THIRDPARTY_MBEDTLS_DIR}library/md5.c
        ${THIRDPARTY_MBEDTLS_DIR}library/memory_buffer_alloc.c
        ${THIRDPARTY_MBEDTLS_DIR}library/mps_reader.c
        ${THIRDPARTY_MBEDTLS_DIR}library/mps_trace.c
        ${THIRDPARTY_MBEDTLS_DIR}library/net_sockets.c
        ${THIRDPARTY_MBEDTLS_DIR}library/nist_kw.c
        ${THIRDPARTY_MBEDTLS_DIR}library/oid.c
        ${THIRDPARTY_MBEDTLS_DIR}library/padlock.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pem.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pk.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pk_ecc.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pk_wrap.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pkcs12.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pkcs5.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pkcs7.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pkparse.c
        ${THIRDPARTY_MBEDTLS_DIR}library/pkwrite.c
        ${THIRDPARTY_MBEDTLS_DIR}library/platform.c
        ${THIRDPARTY_MBEDTLS_DIR}library/platform_util.c
        ${THIRDPARTY_MBEDTLS_DIR}library/poly1305.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_aead.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_cipher.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_client.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_driver_wrappers_no_static.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_ecp.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_ffdh.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_hash.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_mac.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_pake.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_rsa.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_se.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_slot_management.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_crypto_storage.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_its_file.c
        ${THIRDPARTY_MBEDTLS_DIR}library/psa_util.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ripemd160.c
        ${THIRDPARTY_MBEDTLS_DIR}library/rsa.c
        ${THIRDPARTY_MBEDTLS_DIR}library/rsa_alt_helpers.c
        ${THIRDPARTY_MBEDTLS_DIR}library/sha1.c
        ${THIRDPARTY_MBEDTLS_DIR}library/sha3.c
        ${THIRDPARTY_MBEDTLS_DIR}library/sha256.c
        ${THIRDPARTY_MBEDTLS_DIR}library/sha512.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_cache.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_ciphersuites.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_client.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_cookie.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_debug_helpers_generated.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_msg.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_ticket.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls12_client.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls12_server.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls13_client.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls13_generic.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls13_keys.c
        ${THIRDPARTY_MBEDTLS_DIR}library/ssl_tls13_server.c
        ${THIRDPARTY_MBEDTLS_DIR}library/threading.c
        ${THIRDPARTY_MBEDTLS_DIR}library/timing.c
        ${THIRDPARTY_MBEDTLS_DIR}library/version.c
        ${THIRDPARTY_MBEDTLS_DIR}library/version_features.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509_create.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509_crl.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509_crt.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509_csr.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509write.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509write_crt.c
        ${THIRDPARTY_MBEDTLS_DIR}library/x509write_csr.c
	)

	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_MBEDTLS_DIR}include
	)
	# TODO: ninja MSVC fix?
	set(MBEDTLS_CONFIG_PATH "<${CMAKE_SOURCE_DIR}/thirdparty/mbedtls/include/godot_module_mbedtls_config.h>")
	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		MBEDTLS_CONFIG_FILE=${MBEDTLS_CONFIG_PATH}
	)

	clone_library(rex_module_${MODULE_NAME} mbedtls)
	disable_warnings_for_target(mbedtls)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC mbedtls)
	target_link_libraries(rex_core PUBLIC mbedtls)

endif()

file(GLOB MBEDTLS_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${MBEDTLS_SOURCES}
)

if (REX_TESTS)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		TESTS_ENABLED
	)
	file(GLOB MBEDTLS_TESTS_SOURCES tests/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${MBEDTLS_TESTS_SOURCES}
	)
	if (DISABLE_EXCEPTIONS)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
		)
	endif()
endif()
