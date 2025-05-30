if (NOT DISABLE_XR)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	file(GLOB MOBILE_VR_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${MOBILE_VR_SOURCES}
	)
endif()
