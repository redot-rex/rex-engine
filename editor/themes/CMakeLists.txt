# TODO: Need to run python font script.
file(GLOB_RECURSE EDITOR_THEMES_SOURCES *.cpp)
target_sources(rex_editor PRIVATE ${EDITOR_THEMES_SOURCES})
