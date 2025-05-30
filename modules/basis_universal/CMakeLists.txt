add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
set(THIRDPARTY_BASIS_UNIVERSAL_DIR ${CMAKE_SOURCE_DIR}/thirdparty/basis_universal/)

add_library(basis_universal OBJECT
	${THIRDPARTY_BASIS_UNIVERSAL_DIR}transcoder/basisu_transcoder.cpp
)

target_include_directories(rex_module_${MODULE_NAME} INTERFACE
	${THIRDPARTY_BASIS_UNIVERSAL_DIR}
)

if (EDITOR_BUILD)
	add_dependencies(rex_module_${MODULE_NAME}
		rex_module_jpg
		rex_module_tinyexr
	)
	set(ENCODER_SOURCES_DIR ${THIRDPARTY_BASIS_UNIVERSAL_DIR}encoder/)
	target_sources(basis_universal PRIVATE
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
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${CMAKE_SOURCE_DIR}/thirdparty/jpeg-compressor/
		${CMAKE_SOURCE_DIR}/thirdparty/tinyexr/
	)
endif()

if (BUILTIN_ZSTD)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${CMAKE_SOURCE_DIR}/thirdparty/zstd/
	)
endif()

clone_library(rex_module_${MODULE_NAME} basis_universal)
disable_warnings_for_target(basis_universal)

target_compile_definitions(basis_universal PRIVATE
	BASISD_SUPPORT_KTX2_ZSTD=0
	BASISD_SUPPORT_ATC=0  # Proprietary Adreno format not supported by Godot.
	BASISD_SUPPORT_FXT1=0  # Legacy format not supported by Godot.
	BASISD_SUPPORT_PVRTC1=0  # Legacy format not supported by Godot.
	BASISD_SUPPORT_PVRTC2=0  # Legacy format not supported by Godot.
)

target_link_libraries(rex_module_${MODULE_NAME} PUBLIC basis_universal)

file(GLOB BASIS_UNIVERSAL_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${BASIS_UNIVERSAL_SOURCES}
)
