add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
set(THIRDPARTY_ASTCENC_DIR ${CMAKE_SOURCE_DIR}/thirdparty/astcenc/)
add_library(astcenc OBJECT
    ${THIRDPARTY_ASTCENC_DIR}astcenc_averages_and_directions.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_block_sizes.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_color_quantize.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_color_unquantize.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_compress_symbolic.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_compute_variance.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_decompress_symbolic.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_diagnostic_trace.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_entry.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_find_best_partitioning.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_ideal_endpoints_and_weights.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_image.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_integer_sequence.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_mathlib.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_mathlib_softfloat.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_partition_tables.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_percentile_tables.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_pick_best_endpoint_format.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_quantization.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_symbolic_physical.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_weight_align.cpp
    ${THIRDPARTY_ASTCENC_DIR}astcenc_weight_quant_xfer_tables.cpp
)

target_include_directories(rex_module_${MODULE_NAME} INTERFACE
	${THIRDPARTY_ASTCENC_DIR}
)

clone_library(rex_module_${MODULE_NAME} astcenc)
disable_warnings_for_target(astcenc)
if (EDITOR_BUILD)
	target_compile_definitions(astcenc PRIVATE
		ASTCENC_DECOMPRESS_ONLY
	)
endif()
target_link_libraries(rex_module_${MODULE_NAME} PUBLIC astcenc)

file(GLOB ASTCENC_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${ASTCENC_SOURCES}
)
