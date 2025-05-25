file(GLOB_RECURSE WINMIDI_SOURCES
	*.cpp
)
target_sources(rex_engine PRIVATE ${WINMIDI_SOURCES})
