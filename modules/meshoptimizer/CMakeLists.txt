if (NOT DISABLE_3D)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	set(THIRDPARTY_MESHOPTIMIZER_DIR ${CMAKE_SOURCE_DIR}/thirdparty/meshoptimizer/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
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

	file(GLOB MESHOPTIMIZER_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${MESHOPTIMIZER_SOURCES}
	)
endif()
