file(GLOB_RECURSE LIBPNG_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${LIBPNG_SOURCES})
