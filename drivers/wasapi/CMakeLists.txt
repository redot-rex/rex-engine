file(GLOB_RECURSE WASAPI_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${WASAPI_SOURCES})
