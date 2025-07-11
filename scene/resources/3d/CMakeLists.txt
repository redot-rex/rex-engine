set(SCENE_RESOURCES_3D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

target_sources(rex_scene PRIVATE
	${SCENE_RESOURCES_3D_DIR}fog_material.cpp
	${SCENE_RESOURCES_3D_DIR}importer_mesh.cpp
	${SCENE_RESOURCES_3D_DIR}mesh_library.cpp
	${SCENE_RESOURCES_3D_DIR}primitive_meshes.cpp
	${SCENE_RESOURCES_3D_DIR}skin.cpp
	${SCENE_RESOURCES_3D_DIR}sky_material.cpp
	${SCENE_RESOURCES_3D_DIR}world_3d.cpp
)

if (NOT DISABLE_PHYSICS_3D)
	target_sources(rex_scene PRIVATE
		${SCENE_RESOURCES_3D_DIR}box_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}capsule_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}concave_polygon_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}convex_polygon_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}cylinder_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}height_map_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}separation_ray_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}sphere_shape_3d.cpp
		${SCENE_RESOURCES_3D_DIR}world_boundary_shape_3d.cpp
	)
endif()

if (NOT DISABLE_NAVIGATION_3D)
	target_sources(rex_scene PRIVATE
		${SCENE_RESOURCES_3D_DIR}navigation_mesh_source_geometry_data_3d.cpp
	)
endif()
