add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
if (BUILTIN_WSLAY)
	set(THIRDPARTY_WSLAY_DIR ${CMAKE_SOURCE_DIR}/thirdparty/wslay/)
	add_library(wslay OBJECT
		${THIRDPARTY_WSLAY_DIR}wslay_net.c
		${THIRDPARTY_WSLAY_DIR}wslay_event.c
		${THIRDPARTY_WSLAY_DIR}wslay_queue.c
		${THIRDPARTY_WSLAY_DIR}wslay_frame.c
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_WSLAY_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		HAVE_CONFIG_H
	)
	if (REX_PLATFORM STREQUAL "windows")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			HAVE_WINSOCK2_H
		)
	else()
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			HAVE_NETINET_IN_H
		)
	endif()
	clone_library(rex_module_${MODULE_NAME} wslay)
	disable_warnings_for_target(wslay)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC wslay)

endif()

file(GLOB WSLAY_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${WSLAY_SOURCES}
)

if (EDITOR_BUILD)
	file(GLOB WSLAY_EDITOR_SOURCES editor/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${WSLAY_EDITOR_SOURCES}
	)
endif()
