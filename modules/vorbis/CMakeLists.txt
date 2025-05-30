add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

target_link_libraries(rex_module_${MODULE_NAME} PUBLIC rex_module_ogg)

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_ogg
)

if (BUILTIN_LIBVORBIS)
	set(THIRDPARTY_LIBVORBIS ${CMAKE_SOURCE_DIR}/thirdparty/libvorbis/)
	add_library(libvorbis OBJECT
        ${THIRDPARTY_LIBVORBIS}bitrate.c
        ${THIRDPARTY_LIBVORBIS}block.c
        ${THIRDPARTY_LIBVORBIS}codebook.c
        ${THIRDPARTY_LIBVORBIS}envelope.c
        ${THIRDPARTY_LIBVORBIS}floor0.c
        ${THIRDPARTY_LIBVORBIS}floor1.c
        ${THIRDPARTY_LIBVORBIS}info.c
        ${THIRDPARTY_LIBVORBIS}lookup.c
        ${THIRDPARTY_LIBVORBIS}lpc.c
        ${THIRDPARTY_LIBVORBIS}lsp.c
        ${THIRDPARTY_LIBVORBIS}mapping0.c
        ${THIRDPARTY_LIBVORBIS}mdct.c
        ${THIRDPARTY_LIBVORBIS}psy.c
        ${THIRDPARTY_LIBVORBIS}registry.c
        ${THIRDPARTY_LIBVORBIS}res0.c
        ${THIRDPARTY_LIBVORBIS}sharedbook.c
        ${THIRDPARTY_LIBVORBIS}smallft.c
        ${THIRDPARTY_LIBVORBIS}synthesis.c
        ${THIRDPARTY_LIBVORBIS}vorbisfile.c
        ${THIRDPARTY_LIBVORBIS}window.c
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_LIBVORBIS}
	)
	if (BUILTIN_LIBOGG)
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${CMAKE_SOURCE_DIR}/thirdparty/libogg/
		)
	endif()
	clone_library(rex_module_${MODULE_NAME} libvorbis)
	disable_warnings_for_target(libvorbis)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC libvorbis)

endif()

file(GLOB VORBIS_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${VORBIS_SOURCES}
)
