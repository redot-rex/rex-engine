file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
    VideoStreamTheora
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (NOT ARCH MATCHES "^rv.*")
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_ogg
		rex_module_vorbis
	)

	if (BUILTIN_LIBTHEORA)
		set(THIRDPARTY_LIBTHEORA_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libtheora/)
		add_library(libtheora OBJECT
			${THIRDPARTY_LIBTHEORA_DIR}bitpack.c
			${THIRDPARTY_LIBTHEORA_DIR}decinfo.c
			${THIRDPARTY_LIBTHEORA_DIR}decode.c
			${THIRDPARTY_LIBTHEORA_DIR}dequant.c
			${THIRDPARTY_LIBTHEORA_DIR}fragment.c
			${THIRDPARTY_LIBTHEORA_DIR}huffdec.c
			${THIRDPARTY_LIBTHEORA_DIR}idct.c
			${THIRDPARTY_LIBTHEORA_DIR}info.c
			${THIRDPARTY_LIBTHEORA_DIR}internal.c
			${THIRDPARTY_LIBTHEORA_DIR}quant.c
			${THIRDPARTY_LIBTHEORA_DIR}state.c
		)

		if (X86_LIBTHEORA_OPT_GCC)
			target_sources(libtheora PRIVATE
				${THIRDPARTY_LIBTHEORA_DIR}x86/mmxfrag.c
				${THIRDPARTY_LIBTHEORA_DIR}x86/mmxidct.c
				${THIRDPARTY_LIBTHEORA_DIR}x86/mmxstate.c
				${THIRDPARTY_LIBTHEORA_DIR}x86/sse2idct.c
				${THIRDPARTY_LIBTHEORA_DIR}x86/x86cpu.c
				${THIRDPARTY_LIBTHEORA_DIR}x86/x86state.c
			)
		endif()

		if (X86_LIBTHEORA_OPT_VC)
			target_sources(libtheora PRIVATE
				${THIRDPARTY_LIBTHEORA_DIR}x86_vc/mmxfrag.c
				${THIRDPARTY_LIBTHEORA_DIR}x86_vc/mmxidct.c
				${THIRDPARTY_LIBTHEORA_DIR}x86_vc/mmxstate.c
				${THIRDPARTY_LIBTHEORA_DIR}x86_vc/x86cpu.c
				${THIRDPARTY_LIBTHEORA_DIR}x86_vc/x86state.c
			)
		endif()
		if (X86_LIBTHEORA_OPT_GCC OR X86_LIBTHEORA_OPT_VC)
			target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
				OC_X86_ASM
			)
		endif()
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_LIBTHEORA_DIR}
		)
		if (BUILTIN_LIBOGG)
			target_include_directories(rex_module_${MODULE_NAME} INTERFACE
				${CMAKE_SOURCE_DIR}/thirdparty/libogg/
			)
		endif()
		if (BUILTIN_LIBVORBIS)
			target_include_directories(rex_module_${MODULE_NAME} INTERFACE
				${CMAKE_SOURCE_DIR}/thirdparty/libvorbis/
			)
		endif()
		clone_library(rex_module_${MODULE_NAME} libtheora)
		disable_warnings_for_target(libtheora)

		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC libtheora)

	endif()

	file(GLOB THEORA_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${THEORA_SOURCES}
	)
endif()
