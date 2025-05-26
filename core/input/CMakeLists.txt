# TODO: Scons uses a python script to generate inputs

file(GLOB_RECURSE CORE_INPUT_SOURCES *.cpp)
target_sources(rex_core PUBLIC ${CORE_INPUT_SOURCES})
