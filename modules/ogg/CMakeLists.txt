file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	OggPacketSequence
	OggPacketSequencePlayback
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

if (BUILTIN_LIBOGG)
	set(THIRDPARTY_LIBOGG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libogg/)
	add_library(libogg OBJECT
        ${THIRDPARTY_LIBOGG_DIR}bitwise.c
        ${THIRDPARTY_LIBOGG_DIR}framing.c
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_LIBOGG_DIR}
	)
	clone_library(rex_module_${MODULE_NAME} libogg)
	disable_warnings_for_target(libogg)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC libogg)

endif()

file(GLOB OGG_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${OGG_SOURCES}
)
