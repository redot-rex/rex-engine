# TODO: Need to compile shaders

file(GLOB_RECURSE GLES_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${GLES_SOURCES})
