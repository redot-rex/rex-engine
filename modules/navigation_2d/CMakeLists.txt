if (NOT DISABLE_NAVIGATION_2D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	if(BUILTIN_RVO2_2D)
		set(THIRDPARTY_RVO2_2D_DIR ${CMAKE_SOURCE_DIR}/thirdparty/rvo2/rvo2_2d/)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${THIRDPARTY_RVO2_2D_DIR}Agent2d.cpp
			${THIRDPARTY_RVO2_2D_DIR}Obstacle2d.cpp
			${THIRDPARTY_RVO2_2D_DIR}KdTree2d.cpp
			${THIRDPARTY_RVO2_2D_DIR}RVOSimulator2d.cpp
		)
		target_include_directories(rex_module_${MODULE_NAME} PRIVATE
			${THIRDPARTY_RVO2_2D_DIR}
		)
	endif()

	file(GLOB_RECURSE NAVIGATION2D_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${NAVIGATION2D_SOURCES}
	)
endif()
