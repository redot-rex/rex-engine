file(GLOB SCENE_RESOURCES_SOURCES *.cpp)
target_sources(rex_scene PRIVATE ${SCENE_RESOURCES_SOURCES})

set(THIRDPARTY_MISC ${CMAKE_SOURCE_DIR}/thirdparty/misc/)

target_sources(rex_scene PRIVATE
    ${THIRDPARTY_MISC}mikktspace.c
    ${THIRDPARTY_MISC}qoa.c
)

add_subdirectory(2d)

if (NOT DISABLE_3D)
	add_subdirectory(3d)
endif()
