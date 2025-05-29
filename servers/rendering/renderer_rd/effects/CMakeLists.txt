set(THIRDPARTY_AMD_FSR2_DIR ${CMAKE_SOURCE_DIR}/thirdparty/amd-fsr2/)

target_sources(rex_servers PRIVATE
	${THIRDPARTY_AMD_FSR2_DIR}ffx_assert.cpp
	${THIRDPARTY_AMD_FSR2_DIR}ffx_fsr2.cpp
)

target_include_directories(rex_servers PRIVATE
	${THIRDPARTY_AMD_FSR2_DIR}
)

target_compile_definitions(rex_servers PRIVATE
	FFX_GCC
)

file(GLOB_RECURSE SERVERS_RENDERING_RENDERER_RD_EFFECTS_SOURCES *.cpp)
target_sources(rex_servers PRIVATE ${SERVERS_RENDERING_RENDERER_RD_EFFECTS_SOURCES})

if (METAL)
	target_sources(rex_servers PRIVATE
		${THIRDPARTY_AMD_FSR2_DIR}metal_fx.mm
	)
endif()
