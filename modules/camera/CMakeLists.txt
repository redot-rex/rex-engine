if (NOT ${CMAKE_HOST_SYSTEM_NAME} STREQUAL "FreeBSD")
	if (REX_PLATFORM MATCHES "linuxbsd|macos|windows")
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/register_types.cpp
		)
		if (REX_PLATFORM STREQUAL "linuxbsd")
			target_sources(rex_module_${MODULE_NAME} PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/camera_linux.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/camera_feed_linux.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/buffer_decoder.cpp
			)
		elseif(REX_PLATFORM STREQUAL "macos")
			target_sources(rex_module_${MODULE_NAME} PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/camera_macos.mm
			)
		elseif(REX_PLATFORM STREQUAL "windows")
			target_sources(rex_module_${MODULE_NAME} PRIVATE
				${CMAKE_CURRENT_SOURCE_DIR}/camera_win.cpp
			)
		endif()
	endif()
endif()
