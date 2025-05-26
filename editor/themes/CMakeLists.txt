# TODO: Need to run python font script.
file(GLOB_RECURSE EDITOR_THEMES_SOURCES *.cpp)
target_sources(rex_editor PUBLIC ${EDITOR_THEMES_SOURCES})
