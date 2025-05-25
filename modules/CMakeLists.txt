file(GLOB_RECURSE MODULE_SOURCES *.cpp *.h)

target_sources(rex_engine PRIVATE ${MODULE_SOURCES})
