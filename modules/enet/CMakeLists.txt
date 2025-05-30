add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

if (BUILTIN_ENET)
	set(THIRDPARTY_ENET_DIR ${CMAKE_SOURCE_DIR}/thirdparty/enet/)
	add_library(enet OBJECT
        ${THIRDPARTY_ENET_DIR}enet_godot.cpp
        ${THIRDPARTY_ENET_DIR}callbacks.c
        ${THIRDPARTY_ENET_DIR}compress.c
        ${THIRDPARTY_ENET_DIR}host.c
        ${THIRDPARTY_ENET_DIR}list.c
        ${THIRDPARTY_ENET_DIR}packet.c
        ${THIRDPARTY_ENET_DIR}peer.c
        ${THIRDPARTY_ENET_DIR}protocol.c
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_ENET_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		GODOT_ENET
	)

	clone_library(rex_module_${MODULE_NAME} enet)
	disable_warnings_for_target(enet)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC enet)
endif()

file(GLOB ENET_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${ENET_SOURCES}
)
