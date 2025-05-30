if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC
		rex_module_csg
		rex_module_gridmap
	)
	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_csg
		rex_module_gridmap
	)
	file(GLOB GLTF_SOURCES
		*.cpp
		structures/*.cpp
	)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GLTF_SOURCES}
	)
	add_subdirectory(extensions)

	if (EDITOR_BUILD)
		file(GLOB GLTF_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${GLTF_EDITOR_SOURCES}
		)
	endif()
endif()
