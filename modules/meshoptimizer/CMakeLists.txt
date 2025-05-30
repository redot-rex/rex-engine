if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	set(THIRDPARTY_MESHOPTIMIZER_DIR ${CMAKE_SOURCE_DIR}/thirdparty/meshoptimizer/)
	add_library(meshoptimizer OBJECT
		${THIRDPARTY_MESHOPTIMIZER_DIR}allocator.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}clusterizer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}indexcodec.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}indexgenerator.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}overdrawanalyzer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}overdrawoptimizer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}simplifier.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}spatialorder.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}stripifier.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vcacheanalyzer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vcacheoptimizer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vertexcodec.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vertexfilter.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vfetchanalyzer.cpp
		${THIRDPARTY_MESHOPTIMIZER_DIR}vfetchoptimizer.cpp
	)
	clone_library(rex_module_${MODULE_NAME} meshoptimizer)
	disable_warnings_for_target(meshoptimizer)

	target_link_libraries(rex_module_${MODULE_NAME} PUBLIC meshoptimizer)

	file(GLOB MESHOPTIMIZER_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${MESHOPTIMIZER_SOURCES}
	)
endif()
