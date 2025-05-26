add_subdirectory(mbedtls)

file(GLOB_RECURSE MODULE_SOURCES *.cpp)

target_sources(rex_engine PRIVATE ${MODULE_SOURCES})
