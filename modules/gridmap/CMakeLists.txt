if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	file(GLOB GRIDMAP_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GRIDMAP_SOURCES}
	)
	if (EDITOR_BUILD)
		file(GLOB GRIDMAP_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${GRIDMAP_EDITOR_SOURCES}
		)
	endif()
endif()
