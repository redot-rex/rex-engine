if (OPENGL3 AND NOT DISABLE_XR)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	# TODO: Web Stuff

	file(GLOB WEBXR_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${WEBXR_SOURCES}
	)
endif()
