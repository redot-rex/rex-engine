file(GLOB_RECURSE SERVERS_SOURCES *.cpp *.h)

target_sources(rex_engine PRIVATE ${SERVERS_SOURCES})
