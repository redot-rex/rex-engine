file(GLOB SCENE_2D_SOURCES *.cpp)
target_sources(rex_scene PRIVATE ${SCENE_2D_SOURCES})

if (NOT DISABLE_PHYSICS_2D)
	add_subdirectory(physics)
endif()

if (NOT DISABLE_NAVIGATION_2D)
	add_subdirectory(navigation)
endif()
