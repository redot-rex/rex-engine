add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_freetype
	rex_module_msdfgen
	rex_module_svg
)

if ("svg" IN_LIST ENABLED_MODULES)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/inc/
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/src/common/
		${CMAKE_SOURCE_DIR}/thirdparty/thorvg/src/renderer/
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		TVG_STATIC
	)
endif()

if ("msdfgen" IN_LIST ENABLED_MODULES AND BUILTIN_MSDFGEN)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/msdfgen/
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		MSDFGEN_PUBLIC=
	)
endif()

if ("freetype" IN_LIST ENABLED_MODULES AND BUILTIN_FREETYPE)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/freetype/include/
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		FT_CONFIG_OPTION_USE_BROTLI
	)
endif()

file(GLOB TEXT_SERVER_FB_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${TEXT_SERVER_FB_SOURCES}
)
