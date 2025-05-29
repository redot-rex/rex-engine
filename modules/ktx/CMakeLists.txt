add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_basis_universal
)

set(THIRDPARTY_KTX_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libktx/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
    ${THIRDPARTY_KTX_DIR}lib/basis_transcode.cpp
    ${THIRDPARTY_KTX_DIR}lib/checkheader.c
    ${THIRDPARTY_KTX_DIR}lib/filestream.c
    ${THIRDPARTY_KTX_DIR}lib/hashlist.c
    ${THIRDPARTY_KTX_DIR}lib/memstream.c
    ${THIRDPARTY_KTX_DIR}lib/miniz_wrapper.cpp
    ${THIRDPARTY_KTX_DIR}lib/swap.c
    ${THIRDPARTY_KTX_DIR}lib/texture.c
    ${THIRDPARTY_KTX_DIR}lib/texture1.c
    ${THIRDPARTY_KTX_DIR}lib/texture2.c
    ${THIRDPARTY_KTX_DIR}lib/vkformat_check.c
    ${THIRDPARTY_KTX_DIR}lib/vkformat_typesize.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/createdfd.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/colourspaces.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/interpretdfd.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/printdfd.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/queries.c
    ${THIRDPARTY_KTX_DIR}lib/dfdutils/vk2dfd.c
)
target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_KTX_DIR}include/
	${THIRDPARTY_KTX_DIR}utils/
	${THIRDPARTY_KTX_DIR}lib/
	${THIRDPARTY_KTX_DIR}other_include/
	${CMAKE_SOURCE_DIR}/thirdparty/basis_universal/
)
if (BUILTIN_ZSTD)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/zstd/
	)
endif()

if (EDITOR_BUILD)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		MINIZ_HEADER_FILE_ONLY
	)
endif()
if (VULKAN)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/vulkan/include/
	)
else()
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		LIBKTX
	)
endif()
target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
	KHRONOS_STATIC=1
)

file(GLOB KTX_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${KTX_SOURCES}
)
