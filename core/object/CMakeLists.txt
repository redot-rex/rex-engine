# TODO: Scon python scripts

file(GLOB_RECURSE CORE_OBJECT_SOURCES *.cpp)
target_sources(rex_core PRIVATE ${CORE_OBJECT_SOURCES})
