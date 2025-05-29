set(SERVERS_NAVIGATION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

if (NOT DISABLE_NAVIGATION_2D)
	target_sources(rex_servers PRIVATE
		${SERVERS_NAVIGATION_DIR}navigation_path_query_parameters_2d.cpp
		${SERVERS_NAVIGATION_DIR}navigation_path_query_result_2d.cpp
	)
endif()

if (NOT DISABLE_NAVIGATION_3D)
	target_sources(rex_servers PRIVATE
		${SERVERS_NAVIGATION_DIR}navigation_path_query_parameters_3d.cpp
		${SERVERS_NAVIGATION_DIR}navigation_path_query_result_3d.cpp
	)
endif()
