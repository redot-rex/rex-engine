file(GLOB OPENXR_EXTENSIONS_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${OPENXR_EXTENSIONS_SOURCES}
)
if (REX_PLATFORM STREQUAL "android")
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/platform/openxr_android_extension.cpp
	)
endif()
if (VULKAN)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/platform/openxr_vulkan_extension.cpp
	)
endif()
if (METAL)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/platform/openxr_metal_extension.mm
	)
endif()
if (OPENGL3 AND NOT REX_PLATFORM STREQUAL "macos")
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/platform/openxr_opengl_extension.cpp
	)
endif()
if (D3D12)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/platform/openxr_d3d12_extension.cpp
	)
endif()
