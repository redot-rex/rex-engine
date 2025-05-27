file(GLOB_RECURSE VULKAN_SOURCES *.cpp)
target_sources(rex_drivers PRIVATE ${VULKAN_SOURCES})

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty/)
set(THIRDPARTY_VULKAN_INCLUDE_DIR ${THIRDPARTY_DIR}vulkan/include/)
set(THIRDPARTY_VOLK_DIR ${THIRDPARTY_DIR}volk/)

target_include_directories(rex_engine PUBLIC ${THIRDPARTY_VULKAN_INCLUDE_DIR})
target_include_directories(rex_engine PUBLIC ${THIRDPARTY_VULKAN_INCLUDE_DIR})
target_include_directories(rex_engine PUBLIC ${THIRDPARTY_DIR}vulkan/)

if (USE_VOLK)
	target_include_directories(rex_engine PUBLIC ${THIRDPARTY_VOLK_DIR})

	target_compile_definitions(rex_engine PUBLIC
		USE_VOLK
	)
endif()

if (REX_PLATFORM STREQUAL "android")
	target_compile_definitions(rex_engine PUBLIC
		VK_USE_PLATFORM_ANDROID_KHR
	)
elseif(REX_PLATFORM STREQUAL "ios")
	target_compile_definitions(rex_engine PUBLIC
		VK_USE_PLATFORM_IOS_MVK
		VK_USE_PLATFORM_METAL_EXT
	)
elseif(REX_PLATFORM STREQUAL "linuxbsd")
	if (X11)
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_XLIB_KHR
		)
	endif()
	if (WAYLAND)
		target_compile_definitions(rex_engine PUBLIC
			VK_USE_PLATFORM_WAYLAND_KHR
		)
	endif()
elseif(REX_PLATFORM STREQUAL "macos")
	target_compile_definitions(rex_engine PUBLIC
		VK_USE_PLATFORM_MACOS_MVK
		VK_USE_PLATFORM_METAL_EXT
	)
elseif (REX_PLATFORM STREQUAL "windows")
	target_compile_definitions(rex_engine PUBLIC
		VK_USE_PLATFORM_WIN32_KHR
	)
endif()

target_sources(rex_drivers PRIVATE
	${THIRDPARTY_DIR}vulkan/vk_mem_alloc.cpp
)

if (USE_VOLK)
	target_compile_definitions(rex_engine PUBLIC
		VMA_STATIC_VULKAN_FUNCTIONS=1
	)
	target_sources(rex_drivers PRIVATE
		${THIRDPARTY_VOLK_DIR}volk.c
	)
elseif(REX_PLATFORM STREQUAL "android")
    # Our current NDK version only provides old Vulkan headers,
    # so we have to limit VMA.
	target_compile_definitions(rex_engine PUBLIC
		VMA_VULKAN_VERSION=1000000
	)
elseif(REX_PLATFORM STREQUAL "macos"
		OR REX_PLATFORM STREQUAL "ios")
    # MoltenVK supports only Vulkan 1.1 API, limit VMA to the same version.
	target_compile_definitions(rex_engine PUBLIC
		VMA_VULKAN_VERSION=1001000
	)
endif()
