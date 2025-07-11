file(GLOB_RECURSE METAL_SOURCES
	*.mm
)
target_sources(rex_drivers PRIVATE ${METAL_SOURCES})

set(THIRDPARTY_SPIRV_DIR ${CMAKE_SOURCE_DIR}/thirdparty/spirv-cross/)

target_sources(rex_drivers PRIVATE
	${THIRDPARTY_SPIRV_DIR}spirv_cfg.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_cross_util.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_cross.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_parser.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_msl.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_reflect.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_glsl.cpp
	${THIRDPARTY_SPIRV_DIR}spirv_cross_parsed_ir.cpp
)

target_include_directories(rex_drivers PRIVATE
	${THIRDPARTY_SPIRV_DIR}
	${THIRDPARTY_SPIRV_DIR}include/
)
