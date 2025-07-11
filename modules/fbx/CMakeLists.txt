file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	EditorSceneFormatImporterFBX2GLTF
	EditorSceneFormatImporterUFBX
	FBXDocument
	FBXState
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC rex_module_gltf)

	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_gltf
	)
	set(THIRDPARTY_UFBX_DIR ${CMAKE_SOURCE_DIR}/thirdparty/ufbx/)
	add_library(ufbx OBJECT
		${THIRDPARTY_UFBX_DIR}ufbx.c
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_UFBX_DIR}
	)

	clone_library(rex_module_${MODULE_NAME} ufbx)
	disable_warnings_for_target(ufbx)

	target_compile_definitions(ufbx PRIVATE
        UFBX_NO_SUBDIVISION
        UFBX_NO_TESSELLATION
        UFBX_NO_GEOMETRY_CACHE
        UFBX_NO_SCENE_EVALUATION
        UFBX_NO_INDEX_GENERATION
        UFBX_NO_SKINNING_EVALUATION
        UFBX_NO_FORMAT_OBJ
	)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC ufbx)

	file(GLOB FBX_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${FBX_SOURCES}
	)
	# NOTE: structures doesn't exist
	# NOTE: extensions is empty

	if (EDITOR_BUILD)
		file(GLOB FBX_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${FBX_EDITOR_SOURCES}
		)
	endif()
endif()
