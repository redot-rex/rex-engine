target_sources(rex_drivers PRIVATE
	accessibility_driver_accesskit.cpp
)

if (ACCESSKIT_SDK_PATH STREQUAL "")
	if (REX_PLATFORM STREQUAL "windows")
		target_sources(rex_drivers PRIVATE
			dynwrappers/accesskit-dll_wrap.c
		)
	endif()
	if (REX_PLATFORM STREQUAL "macos")
		target_sources(rex_drivers PRIVATE
			dynwrappers/accesskit-dylib_wrap.c
		)
	endif()
	if (REX_PLATFORM STREQUAL "linuxbsd")
		target_sources(rex_drivers PRIVATE
			dynwrappers/accesskit-so_wrap.c
		)
	endif()
	target_include_directories(rex_engine PUBLIC
		{CMAKE_SOURCE_DIR}/thirdparty/accesskit/include/
	)
endif()
