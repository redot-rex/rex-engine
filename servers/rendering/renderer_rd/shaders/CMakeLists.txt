file(GLOB_RECURSE GL_INCLUDE_FILES *_inc.glsl)
file(GLOB_RECURSE GLSL_FILES *.glsl)

if(GL_INCLUDE_FILES)
	list(REMOVE_ITEM GLSL_FILES ${GL_INCLUDE_FILES})
endif()

list(JOIN GLSL_FILES " " SHADER_FILES_ARGS)
list(TRANSFORM GLSL_FILES APPEND ".gen.h")
list(JOIN GL_INCLUDE_FILES " " INCLUDE_FILES_ARGS)
list(TRANSFORM GL_INCLUDE_FILES APPEND ".gen.h")

add_custom_command(
	OUTPUT ${GLSL_FILES}
	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/rex_glsl_builders.py build_rd_headers ${SHADER_FILES_ARGS}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating GLSL Shader Files..."
)
add_custom_command(
	OUTPUT ${GL_INCLUDE_FILES}
	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/rex_glsl_builders.py build_raw_headers ${INCLUDE_FILES_ARGS}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating GLSL Include Files..."
)
add_custom_target(process_shaders DEPENDS
	${GLSL_FILES}
	${GL_INCLUDE_FILES}
)

add_dependencies(rex_servers process_shaders)
