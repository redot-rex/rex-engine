if (EDITOR_BUILD AND NOT REX_PLATFORM MATCHES "android|ios")
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	# TODO: Shaders

	file(GLOB LIGHTMAPPER_RD_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${LIGHTMAPPER_RD_SOURCES}
	)
endif()
