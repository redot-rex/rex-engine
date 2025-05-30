if (EDITOR_BUILD AND NOT REX_PLATFORM MATCHES "android|ios")
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	if (BUILTIN_XATLAS)
		set(THIRDPARTY_XATLAS_DIR ${CMAKE_SOURCE_DIR}/thirdparty/xatlas/)
		add_library(xatlas OBJECT
			${THIRDPARTY_XATLAS_DIR}xatlas.cpp
		)
		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
			${THIRDPARTY_XATLAS_DIR}
		)
		clone_library(rex_module_${MODULE_NAME} xatlas)
		disable_warnings_for_target(xatlas)
		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC xatlas)
	endif()

	file(GLOB XATLAS_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${XATLAS_SOURCES}
	)
endif()
