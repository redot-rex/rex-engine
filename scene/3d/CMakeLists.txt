file(GLOB SCENE_3D_SOURCES *.cpp)
target_sources(rex_scene PRIVATE ${SCENE_3D_SOURCES})

if (NOT DISABLE_PHYSICS_3D)
	add_subdirectory(physics)
endif()

if (NOT DISABLE_NAVIGATION_3D)
	add_subdirectory(navigation)
endif()

if (NOT DISABLE_XR)
	add_subdirectory(xr)
endif()
