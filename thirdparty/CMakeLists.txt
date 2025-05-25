set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(rex_engine PRIVATE
	${THIRDPARTY_DIR}misc/fastlz.c
	${THIRDPARTY_DIR}misc/r128.c
	${THIRDPARTY_DIR}misc/smaz.c
	${THIRDPARTY_DIR}misc/pcg.cpp
	${THIRDPARTY_DIR}misc/polypartition.cpp
	${THIRDPARTY_DIR}misc/smolv.cpp
)


# Minizip library, could be unbundled in theory
# However, our version has some custom modifications, so it won't compile with the system one
set(THIRDPARTY_MINIZIP_DIR ${THIRDPARTY_DIR}minizip/)
target_sources(rex_engine PRIVATE
	${THIRDPARTY_MINIZIP_DIR}ioapi.c
	${THIRDPARTY_MINIZIP_DIR}unzip.c
	${THIRDPARTY_MINIZIP_DIR}zip.c
)

# NOTE: Moved from drivers SCsub
if(REX_PLATFORM STREQUAL "windows")
	set(THIRDPARTY_BACKTRACE_DIR ${THIRDPARTY_DIR}libbacktrace/)
	target_sources(rex_engine PRIVATE
		${THIRDPARTY_BACKTRACE_DIR}atomic.c
		${THIRDPARTY_BACKTRACE_DIR}dwarf.c
		${THIRDPARTY_BACKTRACE_DIR}fileline.c
		${THIRDPARTY_BACKTRACE_DIR}posix.c
		${THIRDPARTY_BACKTRACE_DIR}print.c
		${THIRDPARTY_BACKTRACE_DIR}sort.c
		${THIRDPARTY_BACKTRACE_DIR}state.c
		${THIRDPARTY_BACKTRACE_DIR}backtrace.c
		${THIRDPARTY_BACKTRACE_DIR}simple.c
		${THIRDPARTY_BACKTRACE_DIR}pecoff.c
		${THIRDPARTY_BACKTRACE_DIR}read.c
		${THIRDPARTY_BACKTRACE_DIR}alloc.c
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_BACKTRACE_DIR}
	)
endif()

if (OPENGL3)
	set(THIRDPARTY_GLAD_DIR ${THIRDPARTY_DIR}glad/)
	target_sources(rex_engine PRIVATE
		${THIRDPARTY_GLAD_DIR}gl.c
		# TODO: include egl.c for angle libs
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_GLAD_DIR}
	)

	target_compile_definitions(rex_engine PUBLIC
		GLAD_ENABLED
		EGL_ENABLED
	)
endif()

if (METAL)
	set(THIRDPARTY_SPIRV_DIR ${THIRDPARTY_DIR}spirv-cross/)
	target_sources(rex_engine PRIVATE
		${THIRDPARTY_SPIRV_DIR}spirv_cfg.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_cross_util.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_cross.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_parser.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_msl.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_reflect.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_glsl.cpp
		${THIRDPARTY_SPIRV_DIR}spirv_cross_parsed_ir.cpp
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_SPRIV_DIR}
		${THIRDPARTY_SPIRV_DIR}include/
	)
endif()

if (VULKAN)
	set(THIRDPARTY_VULKAN_DIR ${THIRDPARTY_DIR}vulkan/)
	set(THIRDPARTY_VOLK_DIR ${THIRDPARTY_DIR}volk/)

	target_compile_definitions(rex_engine PUBLIC
		VULKAN_ENABLED
	)

	target_sources(rex_engine PRIVATE
		${THIRDPARTY_VULKAN_DIR}vk_mem_alloc.cpp
	)

	if (REX_PLATFORM STREQUAL "android")
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_ANDROID_KHR
		)
	elseif (REX_PLATFORM STREQUAL "IOS")
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_IOS_MVK
			VK_USE_PLATFORM_METAL_EXT
			VMA_VOLKAN_VERSION=1001000
		)
	elseif (REX_PLATFORM STREQUAL "linux")
		if (X11)
			target_compile_definitions(rex_engine PUBLIC
				VK_USE_PLATFORM_XLIB_KHR
			)
		endif()
		if (WAYLAND)
			target_compile_definitions(rex_engine PUBLIC
				VK_USE_PLATFORM_WAYLAND_KHR
			)
		endif()
	elseif (REX_PLATFORM STREQUAL "macos")
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_MACOS_KHR
			VK_USE_PLATFORM_METAL_EXT
			VMA_VOLKAN_VERSION=1001000
		)
	elseif (REX_PLATFORM STREQUAL "windows")
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_WIN32_KHR
		)
	endif()


	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_VULKAN_DIR}
		${THIRDPARTY_VULKAN_DIR}include/
	)

	if (USE_VOLK)
		target_sources(rex_engine PRIVATE
			${THIRDPARTY_VOLK_DIR}volk.c
		)
		target_include_directories(rex_engine PRIVATE
			${THIRDPARTY_VOLK_DIR}
		)
		target_compile_definitions(rex_engine PUBLIC
			VMA_STATIC_VULKAN_FUNCTIONS=1
		)
	endif()
endif()

if (BUILTIN_BROTLI)
	set(THIRDPARTY_BROTLI_DIR ${THIRDPARTY_DIR}brotli/)
	target_sources(rex_engine PRIVATE
        ${THIRDPARTY_BROTLI_DIR}common/constants.c
		${THIRDPARTY_BROTLI_DIR}common/context.c
		${THIRDPARTY_BROTLI_DIR}common/dictionary.c
        ${THIRDPARTY_BROTLI_DIR}common/platform.c
        ${THIRDPARTY_BROTLI_DIR}common/shared_dictionary.c
        ${THIRDPARTY_BROTLI_DIR}common/transform.c
        ${THIRDPARTY_BROTLI_DIR}dec/bit_reader.c
        ${THIRDPARTY_BROTLI_DIR}dec/decode.c
        ${THIRDPARTY_BROTLI_DIR}dec/huffman.c
        ${THIRDPARTY_BROTLI_DIR}dec/state.c
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_BROTLI_DIR}include
	)
endif()

if (BUILTIN_LIBPNG)
	set(THIRDPARTY_LIBPNG_DIR ${THIRDPARTY_DIR}libpng/)
	target_sources(rex_engine PRIVATE
        ${THIRDPARTY_LIBPNG_DIR}png.c
        ${THIRDPARTY_LIBPNG_DIR}pngerror.c
        ${THIRDPARTY_LIBPNG_DIR}pngget.c
        ${THIRDPARTY_LIBPNG_DIR}pngmem.c
        ${THIRDPARTY_LIBPNG_DIR}pngpread.c
        ${THIRDPARTY_LIBPNG_DIR}pngread.c
        ${THIRDPARTY_LIBPNG_DIR}pngrio.c
        ${THIRDPARTY_LIBPNG_DIR}pngrtran.c
        ${THIRDPARTY_LIBPNG_DIR}pngrutil.c
        ${THIRDPARTY_LIBPNG_DIR}pngset.c
        ${THIRDPARTY_LIBPNG_DIR}pngtrans.c
        ${THIRDPARTY_LIBPNG_DIR}pngwio.c
        ${THIRDPARTY_LIBPNG_DIR}pngwrite.c
        ${THIRDPARTY_LIBPNG_DIR}pngwtran.c
        ${THIRDPARTY_LIBPNG_DIR}pngwutil.c
	)

	if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm.*")
		target_sources(rex_engine PRIVATE
            ${THIRDPARTY_LIBPNG_DIR}arm/arm_init.c
            ${THIRDPARTY_LIBPNG_DIR}arm/filter_neon_intrinsics.c
            ${THIRDPARTY_LIBPNG_DIR}arm/palette_neon_intrinsics.c
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^x86.*")
		target_sources(rex_engine PRIVATE
			${THIRDPARTY_LIBPNG_DIR}intel/intel_init.c
			${THIRDPARTY_LIBPNG_DIR}intel/filter_sse2_intrinsics.c
		)
		target_compile_definitions(rex_engine PUBLIC
			PNG_INTEL_SSE
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "loongarch64")
		target_sources(rex_engine PRIVATE
			${THIRDPARTY_LIBPNG_DIR}loongarch/loongarch_lsx_init.c
			${THIRDPARTY_LIBPNG_DIR}loongarch/filter_lsx_intrinsics.c
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64")
		target_sources(rex_engine PRIVATE
			${THIRDPARTY_LIBPNG_DIR}powerpc/powerpc_init.c
			${THIRDPARTY_LIBPNG_DIR}powerpc/filter_vsx_intrinsics.c
		)
	endif()

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_LIBPNG_DIR}
	)
endif()

if(BUILTIN_MBEDTLS)
	set(THIRDPARTY_MBEDTLS_DIR ${THIRDPARTY_DIR}mbedtls/)
	target_sources(rex_engine PRIVATE
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

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_MBEDTLS_DIR}include
	)
endif()

if (BUILTIN_CLIPPER2)
	set(THIRDPARTY_CLIPPER2_DIR ${THIRDPARTY_DIR}clipper2/)
	target_sources(rex_engine PRIVATE
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.engine.cpp
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.offset.cpp
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.rectclip.cpp
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_CLIPPER2_DIR}include
	)
endif()

if (BUILTIN_ZLIB)
	set(THIRDPARTY_ZLIB_DIR ${THIRDPARTY_DIR}zlib/)
	target_sources(rex_engine PRIVATE
        ${THIRDPARTY_ZLIB_DIR}adler32.c
        ${THIRDPARTY_ZLIB_DIR}compress.c
        ${THIRDPARTY_ZLIB_DIR}crc32.c
        ${THIRDPARTY_ZLIB_DIR}deflate.c
        ${THIRDPARTY_ZLIB_DIR}inffast.c
        ${THIRDPARTY_ZLIB_DIR}inflate.c
        ${THIRDPARTY_ZLIB_DIR}inftrees.c
        ${THIRDPARTY_ZLIB_DIR}trees.c
        ${THIRDPARTY_ZLIB_DIR}uncompr.c
        ${THIRDPARTY_ZLIB_DIR}zutil.c
	)

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_ZLIB_DIR}
	)

	if (REX_DEV_MODE)
		target_compile_definitions(rex_engine PUBLIC ZLIB_DEBUG)
	endif()
endif()

if (BUILTIN_ZSTD)
	set(THIRDPARTY_ZSTD_DIR ${THIRDPARTY_DIR}zstd/)
	target_sources(rex_engine PRIVATE
        ${THIRDPARTY_ZSTD_DIR}common/debug.c
        ${THIRDPARTY_ZSTD_DIR}common/entropy_common.c
        ${THIRDPARTY_ZSTD_DIR}common/error_private.c
        ${THIRDPARTY_ZSTD_DIR}common/fse_decompress.c
        ${THIRDPARTY_ZSTD_DIR}common/pool.c
        ${THIRDPARTY_ZSTD_DIR}common/threading.c
        ${THIRDPARTY_ZSTD_DIR}common/xxhash.c
        ${THIRDPARTY_ZSTD_DIR}common/zstd_common.c
        ${THIRDPARTY_ZSTD_DIR}compress/fse_compress.c
        ${THIRDPARTY_ZSTD_DIR}compress/hist.c
        ${THIRDPARTY_ZSTD_DIR}compress/huf_compress.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_compress.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_double_fast.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_fast.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_lazy.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_ldm.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_opt.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstdmt_compress.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_compress_literals.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_compress_sequences.c
        ${THIRDPARTY_ZSTD_DIR}compress/zstd_compress_superblock.c
        ${THIRDPARTY_ZSTD_DIR}decompress/huf_decompress.c
        ${THIRDPARTY_ZSTD_DIR}decompress/zstd_ddict.c
        ${THIRDPARTY_ZSTD_DIR}decompress/zstd_decompress_block.c
        ${THIRDPARTY_ZSTD_DIR}decompress/zstd_decompress.c
	)
	if (REX_PLATFORM STREQUAL "android"
		OR REX_PLATFORM STREQUAL "ios"
		OR REX_PLATFORM STREQUAL "linux"
		OR REX_PLATFORM STREQUAL "macos")
		target_sources(rex_engine PRIVATE
			${THIRDPARTY_ZSTD_DIR}decompress/huf_decompress_amd64.S
		)
	endif()

	target_include_directories(rex_engine PRIVATE
		${THIRDPARTY_ZSTD_DIR}
		${THIRDPARTY_ZSTD_DIR}common/
	)

	target_compile_definitions(rex_engine PUBLIC ZSTD_STATIC_LINKING_ONLY)

endif()

