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
