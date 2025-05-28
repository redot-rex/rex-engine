set(THIRDPARTY_BASIS_UNIVERSAL_DIR ${CMAKE_SOURCE_DIR}/thirdparty/basis_universal/)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_BASIS_UNIVERSAL_DIR}transcoder/basisu_transcoder.cpp
)

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_BASIS_UNIVERSAL_DIR}
)

if (EDITOR_BUILD)
	set(ENCODER_SOURCES_DIR ${THIRDPARTY_BASIS_UNIVERSAL_DIR}encoder/)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
        ${ENCODER_SOURCES_DIR}3rdparty/android_astc_decomp.cpp
        ${ENCODER_SOURCES_DIR}basisu_astc_hdr_6x6_enc.cpp
        ${ENCODER_SOURCES_DIR}basisu_astc_hdr_common.cpp
        ${ENCODER_SOURCES_DIR}basisu_backend.cpp
        ${ENCODER_SOURCES_DIR}basisu_basis_file.cpp
        ${ENCODER_SOURCES_DIR}basisu_bc7enc.cpp
        ${ENCODER_SOURCES_DIR}basisu_comp.cpp
        ${ENCODER_SOURCES_DIR}basisu_enc.cpp
        ${ENCODER_SOURCES_DIR}basisu_etc.cpp
        ${ENCODER_SOURCES_DIR}basisu_frontend.cpp
        ${ENCODER_SOURCES_DIR}basisu_gpu_texture.cpp
        ${ENCODER_SOURCES_DIR}basisu_kernels_sse.cpp
        ${ENCODER_SOURCES_DIR}basisu_opencl.cpp
        ${ENCODER_SOURCES_DIR}basisu_pvrtc1_4.cpp
        ${ENCODER_SOURCES_DIR}basisu_resample_filters.cpp
        ${ENCODER_SOURCES_DIR}basisu_resampler.cpp
        ${ENCODER_SOURCES_DIR}basisu_ssim.cpp
        ${ENCODER_SOURCES_DIR}basisu_uastc_enc.cpp
        ${ENCODER_SOURCES_DIR}basisu_uastc_hdr_4x4_enc.cpp
        ${ENCODER_SOURCES_DIR}pvpngreader.cpp
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/jpeg-compressor/
		${CMAKE_SOURCE_DIR}/thirdparty/tinyexr/
	)
endif()

if (BUILTIN_ZSTD)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/zstd/
	)
endif()

target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
        BASISD_SUPPORT_KTX2_ZSTD=0
        BASISD_SUPPORT_ATC=0  # Proprietary Adreno format not supported by Godot.
        BASISD_SUPPORT_FXT1=0  # Legacy format not supported by Godot.
        BASISD_SUPPORT_PVRTC1=0  # Legacy format not supported by Godot.
        BASISD_SUPPORT_PVRTC2=0  # Legacy format not supported by Godot.
)

