file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
    UPNP
    UPNPDevice
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

if (BUILTIN_MINIUPNPC AND NOT REX_PLATFORM STREQUAL "web")
	set(THIRDPARTY_MINIUPNPC_DIR ${CMAKE_SOURCE_DIR}/thirdparty/miniupnpc/)
	add_library(miniupnpc OBJECT
        ${THIRDPARTY_MINIUPNPC_DIR}src/igd_desc_parse.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/miniupnpc.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/minixml.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/minisoap.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/minissdpc.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/miniwget.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/upnpcommands.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/upnpdev.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/upnpreplyparse.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/connecthostport.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/portlistingparse.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/receivedata.c
        ${THIRDPARTY_MINIUPNPC_DIR}src/addr_is_reserved.c
	)

	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_MINIUPNPC_DIR}include/
	)

	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		MINIUPNP_STATICLIB
	)
	if (REX_PLATFORM STREQUAL "windows")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			MINIUPNPC_SET_SOCKET_TIMEOUT
		)
	endif()
	clone_library(rex_module_${MODULE_NAME} miniupnpc)
	disable_warnings_for_target(miniupnpc)
	target_include_directories(miniupnpc PRIVATE
		${THIRDPARTY_MINIUPNPC_DIR}include/miniupnpc/
	)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC miniupnpc)
endif()

file(GLOB MINIUPNPC_SOURCES *.cpp)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${MINIUPNPC_SOURCES}
)
