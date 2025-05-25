file(GLOB_RECURSE METAL_SOURCES
	*.mm
)
target_sources(rex_engine PRIVATE ${METAL_SOURCES})
