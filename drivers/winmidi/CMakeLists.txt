file(GLOB_RECURSE WINMIDI_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${WINMIDI_SOURCES})
