file(GLOB_RECURSE CORE_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${CORE_SOURCES})
