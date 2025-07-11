file(GLOB_RECURSE XAUDIO2_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${XAUDIO2_SOURCES})

target_compile_definitions(rex_engine PUBLIC XAUDIO2_ENABLED)

target_link_libraries(rex_drivers PRIVATE xaudio2.8)


