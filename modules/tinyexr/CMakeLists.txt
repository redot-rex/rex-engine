add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

set(THIRDPARTY_TINYEXR_DIR ${CMAKE_SOURCE_DIR}/thirdparty/tinyexr/)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_TINYEXR_DIR}tinyexr.cc
)
target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_TINYEXR_DIR}
)
target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
	TINYEXR_USE_THREAD
	TINYEXR_USE_MINIZ=0
)

file(GLOB TINYEXR_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${TINYEXR_SOURCES}
)
