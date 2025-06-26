file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	EditorSceneFormatImporterBlend
	EditorSceneFormatImporterGLTF
	GLTFAccessor
	GLTFAnimation
	GLTFBufferView
	GLTFCamera
	GLTFDocument
	GLTFDocumentExtension
	GLTFDocumentExtensionConvertImporterMesh
	GLTFLight
	GLTFMesh
	GLTFNode
	GLTFObjectModelProperty
	GLTFPhysicsBody
	GLTFPhysicsShape
	GLTFSkeleton
	GLTFSkin
	GLTFSpecGloss
	GLTFState
	GLTFTexture
	GLTFTextureSampler
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC
		rex_module_csg
		rex_module_gridmap
	)
	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_csg
		rex_module_gridmap
	)
	file(GLOB GLTF_SOURCES
		*.cpp
		structures/*.cpp
	)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GLTF_SOURCES}
	)
	add_subdirectory(extensions)

	if (EDITOR_BUILD)
		file(GLOB GLTF_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${GLTF_EDITOR_SOURCES}
		)
	endif()
endif()
