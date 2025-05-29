if (NOT DISABLE_PHYSICS_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	file(GLOB_RECURSE GODOT_PHYSICS_3D_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GODOT_PHYSICS_3D_SOURCES}
	)
endif()
