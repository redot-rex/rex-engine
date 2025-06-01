
add_subdirectory(shaders)

file(GLOB SERVERS_RENDERING_RENDERER_RD_SOURCES
	*.cpp
	environment/*.cpp
	forward_clustered/*.cpp
	forward_mobile/*.cpp
	storage_rd/*.cpp
)
target_sources(rex_servers PRIVATE ${SERVERS_RENDERING_RENDERER_RD_SOURCES})

add_subdirectory(effects)

set(THIRDPARTY_SPIRV_REFLECT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/spirv-reflect/)

target_sources(rex_servers PRIVATE
	${THIRDPARTY_SPIRV_REFLECT_DIR}spirv_reflect.c
)
