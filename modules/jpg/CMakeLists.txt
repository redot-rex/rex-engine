add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

set(THIRDPARTY_JPG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/jpeg-compressor/)
add_library(jpg OBJECT
    ${THIRDPARTY_JPG_DIR}jpgd.cpp
    ${THIRDPARTY_JPG_DIR}jpge.cpp
)

target_include_directories(rex_module_${MODULE_NAME} INTERFACE
	${THIRDPARTY_JPG_DIR}
)
clone_library(rex_module_${MODULE_NAME} jpg)
disable_warnings_for_target(jpg)

target_link_libraries(rex_module_${MODULE_NAME} PUBLIC jpg)

file(GLOB_RECURSE JPG_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${JPG_SOURCES}
)
