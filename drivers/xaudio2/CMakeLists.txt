file(GLOB_RECURSE XAUDIO2_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${XAUDIO2_SOURCES})

target_compile_definitions(rex_engine PRIVATE XAUDIO2_ENABLED)

target_link_libraries(rex_engine PRIVATE xaudio2.8)


