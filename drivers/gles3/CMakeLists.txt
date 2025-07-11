# as we have a few, not yet, converted files we name the ones we want to include:

#file(GLOB_RECURSE GL_INCLUDE_FILES *_inc.glsl)
#file(GLOB_RECURSE GLSL_FILES *.glsl)

#if(GL_INCLUDE_FILES)
#list(REMOVE_ITEM GLSL_FILES ${GL_INCLUDE_FILES})
#endif()
set(GLSL_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/canvas.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/feed.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/scene.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/sky.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/canvas_occlusion.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/canvas_sdf.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/particles.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/particles_copy.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/skeleton.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effects/copy.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effects/cubemap_filter.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effects/glow.glsl
	${CMAKE_CURRENT_SOURCE_DIR}/shaders/effects/post.glsl
)

list(JOIN GLSL_FILES " " SHADER_FILES_ARGS)
list(TRANSFORM GLSL_FILES APPEND ".gen.h")
#list(JOIN GL_INCLUDE_FILES " " INCLUDE_FILES_ARGS)
#list(TRANSFORM GL_INCLUDE_FILES APPEND ".gen.h")


add_custom_command(
	OUTPUT ${GLSL_FILES}
	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generators/rex_gles3_builders.py build_gles3_headers ${SHADER_FILES_ARGS}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating GLES3 Shader Files..."
)
#add_custom_command(
#	OUTPUT ${GL_INCLUDE_FILES}
#	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generators/rex_glsl_builders.py build_raw_headers ${INCLUDE_FILES_ARGS}
#	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#	COMMENT "Generating GLSL Include Files..."
#)
add_custom_target(process_drivers_shaders DEPENDS
	${GLSL_FILES}
	#	${GL_INCLUDE_FILES}
)

add_dependencies(rex_drivers process_drivers_shaders)

file(GLOB_RECURSE GLES_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${GLES_SOURCES})
