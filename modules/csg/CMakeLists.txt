file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	CSGBox3D
	CSGCombiner3D
	CSGCylinder3D
	CSGMesh3D
	CSGPolygon3D
	CSGPrimitive3D
	CSGShape3D
	CSGSphere3D
	CSGTorus3D
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		MANIFOLD_PAR=-1
	)

	set(THIRDPARTY_MANIFOLD_DIR ${CMAKE_SOURCE_DIR}/thirdparty/manifold/)

	add_library(manifold OBJECT
		${THIRDPARTY_MANIFOLD_DIR}src/boolean_result.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/boolean3.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/constructors.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/csg_tree.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/edge_op.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/face_op.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/impl.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/manifold.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/polygon.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/properties.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/quickhull.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/smoothing.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/sort.cpp
		${THIRDPARTY_MANIFOLD_DIR}src/subdivision.cpp
	)

	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_MANIFOLD_DIR}include/
	)

	clone_library(rex_module_${MODULE_NAME} manifold)
	disable_warnings_for_target(manifold)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC manifold)
	file(GLOB CSG_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CSG_SOURCES}
	)

	if (EDITOR_BUILD)
		file(GLOB CSG_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${CSG_EDITOR_SOURCES}
		)
	endif()
endif()
