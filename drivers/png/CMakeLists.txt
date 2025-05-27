file(GLOB_RECURSE LIBPNG_SOURCES
	*.cpp
)
target_sources(rex_drivers PRIVATE ${LIBPNG_SOURCES})

if (BUILTIN_LIBPNG)
	set(THIRDPARTY_LIBPNG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/libpng/)
	target_sources(rex_drivers PRIVATE
        ${THIRDPARTY_LIBPNG_DIR}png.c
        ${THIRDPARTY_LIBPNG_DIR}pngerror.c
        ${THIRDPARTY_LIBPNG_DIR}pngget.c
        ${THIRDPARTY_LIBPNG_DIR}pngmem.c
        ${THIRDPARTY_LIBPNG_DIR}pngpread.c
        ${THIRDPARTY_LIBPNG_DIR}pngread.c
        ${THIRDPARTY_LIBPNG_DIR}pngrio.c
        ${THIRDPARTY_LIBPNG_DIR}pngrtran.c
        ${THIRDPARTY_LIBPNG_DIR}pngrutil.c
        ${THIRDPARTY_LIBPNG_DIR}pngset.c
        ${THIRDPARTY_LIBPNG_DIR}pngtrans.c
        ${THIRDPARTY_LIBPNG_DIR}pngwio.c
        ${THIRDPARTY_LIBPNG_DIR}pngwrite.c
        ${THIRDPARTY_LIBPNG_DIR}pngwtran.c
        ${THIRDPARTY_LIBPNG_DIR}pngwutil.c
	)

	if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^arm.*")
		target_sources(rex_drivers PRIVATE
            ${THIRDPARTY_LIBPNG_DIR}arm/arm_init.c
            ${THIRDPARTY_LIBPNG_DIR}arm/filter_neon_intrinsics.c
            ${THIRDPARTY_LIBPNG_DIR}arm/palette_neon_intrinsics.c
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "^x86.*")
		target_sources(rex_drivers PRIVATE
			${THIRDPARTY_LIBPNG_DIR}intel/intel_init.c
			${THIRDPARTY_LIBPNG_DIR}intel/filter_sse2_intrinsics.c
		)
		target_compile_definitions(rex_drivers PUBLIC
			PNG_INTEL_SSE
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "loongarch64")
		target_sources(rex_drivers PRIVATE
			${THIRDPARTY_LIBPNG_DIR}loongarch/loongarch_lsx_init.c
			${THIRDPARTY_LIBPNG_DIR}loongarch/filter_lsx_intrinsics.c
		)
	elseif(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ppc64")
		target_sources(rex_drivers PRIVATE
			${THIRDPARTY_LIBPNG_DIR}powerpc/powerpc_init.c
			${THIRDPARTY_LIBPNG_DIR}powerpc/filter_vsx_intrinsics.c
		)
	endif()

	target_include_directories(rex_drivers PRIVATE
		${THIRDPARTY_LIBPNG_DIR}
	)
endif()
