if (EDITOR_BUILD)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})
	file(GLOB BETSY_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${BETSY_SOURCES}
	)

	# TODO: Shader compilation

endif()
