set(SERVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

add_library(rex_servers STATIC
	${SERVERS_DIR}audio_server.cpp
	${SERVERS_DIR}camera_server.cpp
	${SERVERS_DIR}display_server.cpp
	${SERVERS_DIR}navigation_server_2d.cpp
	${SERVERS_DIR}navigation_server_3d.cpp
	${SERVERS_DIR}register_server_types.cpp
	${SERVERS_DIR}rendering_server.cpp
	${SERVERS_DIR}text_server.cpp
)

target_include_directories(rex_servers PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_servers PUBLIC ${ENGINE_DEFINITIONS})

add_subdirectory(audio)
add_subdirectory(camera)
add_subdirectory(debugger)
add_subdirectory(display)
add_subdirectory(extensions)
add_subdirectory(movie_writer)
add_subdirectory(navigation)
add_subdirectory(rendering)
add_subdirectory(text)


if (NOT DISABLE_PHYSICS_2D)
	target_sources(rex_servers PRIVATE
		${SERVERS_DIR}physics_server_2d.cpp
		${SERVERS_DIR}physics_server_2d_wrap_mt.cpp
	)
endif()

if (NOT DISABLE_PHYSICS_3D)
	target_sources(rex_servers PRIVATE
		${SERVERS_DIR}physics_server_3d.cpp
		${SERVERS_DIR}physics_server_3d_wrap_mt.cpp
	)
endif()

if (NOT DISABLE_XR)
	target_sources(rex_servers PRIVATE
		${SERVERS_DIR}xr_server.cpp
	)
	add_subdirectory(xr)
endif()

#target_link_libraries(rex_engine PUBLIC rex_servers)
