file(GLOB_RECURSE WINDOWS_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${WINDOWS_SOURCES})
