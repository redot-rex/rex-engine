if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	set(THIRDPARTY_VHACD_DIR ${CMAKE_SOURCE_DIR}/thirdparty/vhacd/)
	add_library(vhacd OBJECT
		${THIRDPARTY_VHACD_DIR}src/vhacdManifoldMesh.cpp
		${THIRDPARTY_VHACD_DIR}src/FloatMath.cpp
		${THIRDPARTY_VHACD_DIR}src/vhacdMesh.cpp
		${THIRDPARTY_VHACD_DIR}src/vhacdICHull.cpp
		${THIRDPARTY_VHACD_DIR}src/vhacdVolume.cpp
		${THIRDPARTY_VHACD_DIR}src/VHACD-ASYNC.cpp
		${THIRDPARTY_VHACD_DIR}src/btAlignedAllocator.cpp
		${THIRDPARTY_VHACD_DIR}src/vhacdRaycastMesh.cpp
		${THIRDPARTY_VHACD_DIR}src/VHACD.cpp
		${THIRDPARTY_VHACD_DIR}src/btConvexHullComputer.cpp
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${THIRDPARTY_VHACD_DIR}inc/
	)
	clone_library(rex_module_${MODULE_NAME} vhacd)
	disable_warnings_for_target(vhacd)
	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC vhacd)
	file(GLOB VHACD_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${VHACD_SOURCES}
	)
endif()
