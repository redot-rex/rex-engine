add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})
if (BUILTIN_WSLAY)
	set(THIRDPARTY_WSLAY_DIR ${CMAKE_SOURCE_DIR}/thirdparty/wslay/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_WSLAY_DIR}wslay_net.c
		${THIRDPARTY_WSLAY_DIR}wslay_event.c
		${THIRDPARTY_WSLAY_DIR}wslay_queue.c
		${THIRDPARTY_WSLAY_DIR}wslay_frame.c
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_WSLAY_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		HAVE_CONFIG_H
	)
	if (REX_PLATFORM STREQUAL "windows")
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			HAVE_WINSOCK2_H
		)
	else()
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			HAVE_NETINET_IN_H
		)
	endif()
endif()

file(GLOB WSLAY_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${WSLAY_SOURCES}
)

if (EDITOR_BUILD)
	file(GLOB WSLAY_EDITOR_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${WSLAY_EDITOR_SOURCES}
	)
endif()
