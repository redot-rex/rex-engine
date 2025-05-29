add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

if (BUILTIN_ENET)
	set(THIRDPARTY_ENET_DIR ${CMAKE_SOURCE_DIR}/thirdparty/enet/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
        ${THIRDPARTY_ENET_DIR}enet_godot.cpp
        ${THIRDPARTY_ENET_DIR}callbacks.c
        ${THIRDPARTY_ENET_DIR}compress.c
        ${THIRDPARTY_ENET_DIR}host.c
        ${THIRDPARTY_ENET_DIR}list.c
        ${THIRDPARTY_ENET_DIR}packet.c
        ${THIRDPARTY_ENET_DIR}peer.c
        ${THIRDPARTY_ENET_DIR}protocol.c
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_ENET_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		GODOT_ENET
	)
endif()

file(GLOB ENET_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${ENET_SOURCES}
)
