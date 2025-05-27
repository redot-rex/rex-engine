file(GLOB_RECURSE WASAPI_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${WASAPI_SOURCES})
