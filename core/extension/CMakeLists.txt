# TODO: Scon wrapper python script

file(GLOB_RECURSE CORE_EXTENSION_SOURCES *.cpp)
target_sources(rex_core PRIVATE ${CORE_EXTENSION_SOURCES})
