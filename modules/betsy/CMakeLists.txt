if (EDITOR_BUILD)
	file(GLOB BETSY_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${BETSY_SOURCES}
	)

	# TODO: Shader compilation

endif()
