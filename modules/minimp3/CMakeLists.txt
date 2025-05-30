add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

option(MINIMP3_EXTRA_FORMATS "Build minimp3 with MP1/MP2 decoding support" OFF)

set(THIRDPARTY_MINIMP3_DIR ${CMAKE_SOURCE_DIR}/thirdparty/minimp3/)
target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_MINIMP3_DIR}
)

if (NOT MINIMP3_EXTRA_FORMATS)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		MINIMP3_ONLY_MP3
	)
endif()

file(GLOB MINIMP3_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${MINIMP3_SOURCES}
)
