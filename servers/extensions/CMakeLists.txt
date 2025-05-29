set(SERVERS_EXTENSIONS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(rex_servers PRIVATE
	${SERVERS_EXTENSIONS_DIR}physics_server_2d_extension.cpp
)

if (NOT DISABLE_3D)
	target_sources(rex_servers PRIVATE
		${SERVERS_EXTENSIONS_DIR}physics_server_3d_extension.cpp
	)
endif()
