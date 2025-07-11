add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

set(THIRDPARTY_TINYEXR_DIR ${CMAKE_SOURCE_DIR}/thirdparty/tinyexr/)

add_library(tinyexr OBJECT
	${THIRDPARTY_TINYEXR_DIR}tinyexr.cc
)
target_include_directories(rex_module_${MODULE_NAME} INTERFACE
	${THIRDPARTY_TINYEXR_DIR}
)
target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
	TINYEXR_USE_THREAD
	TINYEXR_USE_MINIZ=0
)
clone_library(rex_module_${MODULE_NAME} tinyexr)
disable_warnings_for_target(tinyexr)

target_link_libraries(rex_module_${MODULE_NAME} PUBLIC tinyexr)

file(GLOB TINYEXR_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${TINYEXR_SOURCES}
)
