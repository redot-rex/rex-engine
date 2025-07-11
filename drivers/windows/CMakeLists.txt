file(GLOB_RECURSE WINDOWS_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${WINDOWS_SOURCES})
