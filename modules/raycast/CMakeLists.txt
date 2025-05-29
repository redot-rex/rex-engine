if (ARCH MATCHES "x86_64|arm64|wasm32" OR (ARCH STREQUAL "x86_32" AND REX_PLATFORM STREQUAL "windows"))
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	disable_warnings_for_target(rex_module_${MODULE_NAME})

	if (BUILTIN_EMBREE)
		set(THIRDPARTY_EMBREE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/embree/)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${THIRDPARTY_EMBREE_DIR}common/sys/sysinfo.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/alloc.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/estring.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/filename.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/library.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/thread.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/regression.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/mutex.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/condition.cpp
			${THIRDPARTY_EMBREE_DIR}common/sys/barrier.cpp
			${THIRDPARTY_EMBREE_DIR}common/math/constants.cpp
			${THIRDPARTY_EMBREE_DIR}common/simd/sse.cpp
			${THIRDPARTY_EMBREE_DIR}common/lexers/stringstream.cpp
			${THIRDPARTY_EMBREE_DIR}common/lexers/tokenstream.cpp
			${THIRDPARTY_EMBREE_DIR}common/tasking/taskschedulerinternal.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/device.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/stat.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/acceln.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/accelset.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/state.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/rtcore.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/rtcore_builder.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/scene.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/scene_verify.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/alloc.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/geometry.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/common/scene_triangle_mesh.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/geometry/primitive4.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/builders/primrefgen.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_statistics.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh4_factory.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh8_factory.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_collider.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_rotate.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_refit.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder_morton.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder_sah.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder_sah_spatial.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder_sah_mb.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_builder_twolevel.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_intersector1_bvh4.cpp
			${THIRDPARTY_EMBREE_DIR}kernels/bvh/bvh_intersector_hybrid4_bvh4.cpp
		)
		target_include_directories(rex_module_${MODULE_NAME} PRIVATE
			${THIRDPARTY_EMBREE_DIR}include/
		)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			EMBREE_TARGET_SSE2
			EMBREE_LOWEST_ISA
			TASKING_INTERNAL
			NDEBUG
		)

		if (NOT ARCH STREQUAL "x86_64" OR MSVC)
			target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
				__SSE2__
				__SSE__
			)
		endif()

		if (NOT MSVC)
			# Flags synced with upstream gnu.cmake.
			if (ARCH STREQUAL "arm64" AND REX_PLATFORM STREQUAL "linuxbsd" AND NOT USE_LLVM)
				target_compile_options(rex_module_${MODULE_NAME} PRIVATE
					-flax-vector-conversions
				)
			endif()
			target_compile_options(rex_module_${MODULE_NAME} PRIVATE
                -fno-strict-overflow
                -fno-delete-null-pointer-checks
                -fwrapv
                -fsigned-char
                -fno-strict-aliasing
                -fno-tree-vectorize
                -fvisibility=hidden
                -fvisibility-inlines-hidden
            )
		endif()
	endif()

	file(GLOB RAYCAST_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${RAYCAST_SOURCES}
	)
endif()
