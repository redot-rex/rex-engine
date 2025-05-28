set(THIRDPARTY_ASTCENC_DIR ${CMAKE_SOURCE_DIR}/thirdparty/astcenc/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
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

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_ASTCENC_DIR}
)

if (EDITOR_BUILD)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		ASTCENC_DECOMPRESS_ONLY
	)
endif()
