file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	ZIPReader
    ZIPPacker
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (MINIZIP)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)
	file(GLOB ZIP_SOURCES *.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${ZIP_SOURCES}
	)
	if (REX_TESTS)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			TESTS_ENABLED
		)
		file(GLOB ZIP_TESTS_SOURCES tests/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${ZIP_TESTS_SOURCES}
		)
	endif()
endif()
