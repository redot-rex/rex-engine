set(SCENE_RESOURCES_2D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(rex_scene PRIVATE
	${SCENE_RESOURCES_2D_DIR}tile_set.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_ccdik.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_fabrik.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_lookat.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_stackholder.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_twoboneik.cpp
	${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_stack_2d.cpp
)

if (NOT DISABLE_PHYSICS_2D)
	target_sources(rex_scene PRIVATE
		${SCENE_RESOURCES_2D_DIR}capsule_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}circle_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}concave_polygon_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}convex_polygon_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}rectangle_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}segment_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}separation_ray_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}world_boundary_shape_2d.cpp
		${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_jiggle.cpp
		${SCENE_RESOURCES_2D_DIR}skeleton/skeleton_modification_2d_physicalbones.cpp
	)
endif()
if (NOT DISABLE_NAVIGATION_2D)
	target_sources(rex_scene PRIVATE
		${SCENE_RESOURCES_2D_DIR}navigation_mesh_source_geometry_data_2d.cpp
		${SCENE_RESOURCES_2D_DIR}navigation_polygon.cpp
		${SCENE_RESOURCES_2D_DIR}polygon_path_finder.cpp
	)
endif()

