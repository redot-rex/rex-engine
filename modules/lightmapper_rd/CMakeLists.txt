if (EDITOR_BUILD AND NOT REX_PLATFORM MATCHES "android|ios")
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	file(GLOB GLSL_FILES *.glsl)
	list(JOIN GLSL_FILES " " SHADER_FILES_ARGS)
	list(TRANSFORM GLSL_FILES APPEND ".gen.h")
	add_custom_command(
		OUTPUT ${GLSL_FILES}
		COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generators/rex_glsl_builders.py build_raw_headers ${SHADER_FILES_ARGS}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating lightmapper_rd GLSL Include Files..."
	)
	add_custom_target(process_lightmapper_rd_shaders DEPENDS
		${GLSL_FILES}
	)

	add_dependencies(rex_servers process_lightmapper_rd_shaders)

	file(GLOB LIGHTMAPPER_RD_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${LIGHTMAPPER_RD_SOURCES}
	)
endif()
