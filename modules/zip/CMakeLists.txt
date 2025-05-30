if (MINIZIP)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	file(GLOB ZIP_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${ZIP_SOURCES}
	)
	if (TESTS)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			TESTS_ENABLED
		)
		file(GLOB ZIP_TESTS_SOURCES tests/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${ZIP_TESTS_SOURCES}
		)
	endif()
endif()
