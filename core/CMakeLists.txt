file(GLOB CORE_SOURCES *.cpp)

add_library(rex_core STATIC ${CORE_SOURCES})

target_include_directories(rex_core PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_core PRIVATE ${ENGINE_DEFINITIONS})

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty/)

add_library(misc OBJECT
	${THIRDPARTY_DIR}misc/fastlz.c
	${THIRDPARTY_DIR}misc/r128.c
	${THIRDPARTY_DIR}misc/smaz.c
	${THIRDPARTY_DIR}misc/pcg.cpp
	${THIRDPARTY_DIR}misc/polypartition.cpp
	${THIRDPARTY_DIR}misc/smolv.cpp
)
clone_library(rex_core misc)
disable_warnings_for_target(misc)
target_link_libraries(rex_core PUBLIC misc)

if (BUILTIN_BROTLI)
	set(THIRDPARTY_BROTLI_DIR ${THIRDPARTY_DIR}brotli/)
	add_library(brotli OBJECT
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

	target_include_directories(rex_engine INTERFACE
		${THIRDPARTY_BROTLI_DIR}include
	)

	if (USE_UBSAN OR USE_ASAN OR USE_TSAN OR USE_LSAN OR USE_MSAN)
		target_compile_definitions(brotli PRIVATE
			BROTLI_BUILD_PORTABLE
		)
	endif()

	clone_library(rex_engine brotli)
	disable_warnings_for_target(brotli)
	target_link_libraries(rex_core PUBLIC brotli)
endif()

if (BUILTIN_CLIPPER2)
	set(THIRDPARTY_CLIPPER2_DIR ${THIRDPARTY_DIR}clipper2/)
	add_library(clipper2 OBJECT
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.engine.cpp
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.offset.cpp
        ${THIRDPARTY_CLIPPER2_DIR}src/clipper.rectclip.cpp
	)

	target_include_directories(rex_engine INTERFACE
		${THIRDPARTY_CLIPPER2_DIR}include
	)

	target_compile_definitions(rex_engine INTERFACE
		CLIPPER2_ENABLED
	)

	clone_library(rex_engine clipper2)
	disable_warnings_for_target(clipper2)
	target_link_libraries(rex_core PUBLIC clipper2)
endif()

if (BUILTIN_ZLIB)
	set(THIRDPARTY_ZLIB_DIR ${THIRDPARTY_DIR}zlib/)
	add_library(zlib OBJECT
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

	target_include_directories(rex_engine INTERFACE
		${THIRDPARTY_ZLIB_DIR}
	)

	if (DEV_BUILD)
		target_compile_definitions(rex_engine PUBLIC ZLIB_DEBUG)
	endif()
	clone_library(rex_engine zlib)
	disable_warnings_for_target(zlib)
	target_link_libraries(rex_core PUBLIC zlib)
endif()

# Minizip library, could be unbundled in theory
# However, our version has some custom modifications, so it won't compile with the system one
set(THIRDPARTY_MINIZIP_DIR ${THIRDPARTY_DIR}minizip/)
add_library(minizip OBJECT
	${THIRDPARTY_MINIZIP_DIR}ioapi.c
	${THIRDPARTY_MINIZIP_DIR}unzip.c
	${THIRDPARTY_MINIZIP_DIR}zip.c
)
clone_library(rex_engine minizip)
disable_warnings_for_target(minizip)
target_link_libraries(rex_core PUBLIC minizip)

if (BUILTIN_ZSTD)
	set(THIRDPARTY_ZSTD_DIR ${THIRDPARTY_DIR}zstd/)
	add_library(zstd OBJECT
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
	if (REX_PLATFORM MATCHES "android|ios|linuxbsd|macos" AND ARCH STREQUAL "x86_64")
		target_sources(zstd PRIVATE
			${THIRDPARTY_ZSTD_DIR}decompress/huf_decompress_amd64.S
		)
		enable_language(ASM)
		set_source_files_properties(${THIRDPARTY_ZSTD_DIR}decompress/huf_decompress_amd64.S PROPERTIES LANGUAGE ASM)
	endif()
	target_include_directories(zstd PRIVATE
		${THIRDPARTY_ZSTD_DIR}common/
	)

	target_include_directories(rex_engine INTERFACE
		${THIRDPARTY_ZSTD_DIR}
	)

	target_compile_definitions(rex_engine PUBLIC ZSTD_STATIC_LINKING_ONLY)

	clone_library(rex_engine zstd)
	disable_warnings_for_target(zstd)
	target_link_libraries(rex_core PUBLIC zstd)
endif()

add_subdirectory(os)
add_subdirectory(math)
add_subdirectory(crypto)
add_subdirectory(io)
add_subdirectory(debugger)
add_subdirectory(input)
add_subdirectory(variant)
add_subdirectory(extension)
add_subdirectory(object)
add_subdirectory(templates)
add_subdirectory(string)
add_subdirectory(config)
add_subdirectory(error)


#target_link_libraries(rex_engine PUBLIC rex_core)
