file(GLOB_RECURSE SCENE_SOURCES *.cpp *.h)

target_sources(rex_engine PRIVATE ${SCENE_SOURCES})
