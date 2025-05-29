file(GLOB SERVERS_RENDERING_SOURCES *.cpp)
target_sources(rex_servers PRIVATE ${SERVERS_RENDERING_SOURCES})

add_subdirectory(dummy)
add_subdirectory(renderer_rd)
add_subdirectory(storage)
