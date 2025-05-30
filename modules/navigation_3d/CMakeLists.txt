if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_csg
		rex_module_gridmap
	)

	if (BUILTIN_RECASTNAVIGATION)
		set(THIRDPARTY_RECASTNAVIGATION_DIR ${CMAKE_SOURCE_DIR}/thirdparty/recastnavigation/Recast/)
		add_library(recastnavigation OBJECT
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/Recast.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastAlloc.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastArea.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastAssert.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastContour.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastFilter.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastLayers.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastMesh.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastMeshDetail.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastRasterization.cpp
			${THIRDPARTY_RECASTNAVIGATION_DIR}Source/RecastRegion.cpp
		)
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_RECASTNAVIGATION_DIR}Include/
		)
		clone_library(rex_module_${MODULE_NAME} recastnavigation)
		disable_warnings_for_target(recastnavigation)

		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC recastnavigation)

	endif()

	if (BUILTIN_RVO2_2D)
		set(THIRDPARTY_RVO2_2D_DIR ${CMAKE_SOURCE_DIR}/thirdparty/rvo2/rvo2_2d/)
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_RVO2_2D_DIR}
		)
		if (NOT "navigation_2d" IN_LIST ENABLED_MODULES)
			add_library(rvo2_2d OBJECT
				${THIRDPARTY_RVO2_2D_DIR}Agent2d.cpp
				${THIRDPARTY_RVO2_2D_DIR}Obstacle2d.cpp
				${THIRDPARTY_RVO2_2D_DIR}KdTree2d.cpp
				${THIRDPARTY_RVO2_2D_DIR}RVOSimulator2d.cpp
			)
			clone_library(rex_module_${MODULE_NAME} rvo2_2d)
			disable_warnings_for_target(rvo2_2d)

			target_link_libraries(rex_module_${MODULE_NAME} PUBLIC rvo2_2d)

		endif()
	endif()

	if (BUILTIN_RVO2_3D)
		set(THIRDPARTY_RVO2_3D_DIR ${CMAKE_SOURCE_DIR}/thirdparty/rvo2/rvo2_3d/)
		add_library(rvo2_3d OBJECT
			${THIRDPARTY_RVO2_3D_DIR}Agent3d.cpp
			${THIRDPARTY_RVO2_3D_DIR}KdTree3d.cpp
			${THIRDPARTY_RVO2_3D_DIR}RVOSimulator3d.cpp
		)
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_RVO2_3D_DIR}
		)
		clone_library(rex_module_${MODULE_NAME} rvo2_3d)
		disable_warnings_for_target(rvo2_3d)
		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC rvo2_3d)
	endif()

	file(GLOB NAVIGATION_3D_SOURCES
		*.cpp
		3d/*.cpp
	)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${NAVIGATION_3D_SOURCES}
	)
	if (EDITOR_BUILD)
		file(GLOB NAVIGATION_3D_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${NAVIGATION_3D_EDITOR_SOURCES}
		)
	endif()
endif()
