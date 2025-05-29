if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})
	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_gltf
	)
	set(THIRDPARTY_UFBX_DIR ${CMAKE_SOURCE_DIR}/thirdparty/ufbx/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_UFBX_DIR}ufbx.c
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_UFBX_DIR}
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
        UFBX_NO_SUBDIVISION
        UFBX_NO_TESSELLATION
        UFBX_NO_GEOMETRY_CACHE
        UFBX_NO_SCENE_EVALUATION
        UFBX_NO_INDEX_GENERATION
        UFBX_NO_SKINNING_EVALUATION
        UFBX_NO_FORMAT_OBJ
	)

	file(GLOB FBX_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${FBX_SOURCES}
	)
	# NOTE: structures doesn't exist
	# NOTE: extensions is empty

	if (EDITOR_BUILD)
		file(GLOB FBX_EDITOR_SOURCES *.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${FBX_EDITOR_SOURCES}
		)
	endif()
endif()
