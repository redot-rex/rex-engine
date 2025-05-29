add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

set(THIRDPARTY_JPG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/jpeg-compressor/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
    ${THIRDPARTY_JPG_DIR}jpgd.cpp
    ${THIRDPARTY_JPG_DIR}jpge.cpp
)

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_JPG_DIR}
)

file(GLOB_RECURSE JPG_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${JPG_SOURCES}
)
