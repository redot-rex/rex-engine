file(GLOB_RECURSE UNIX_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${UNIX_SOURCES})
