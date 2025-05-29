add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

set(THIRDPARTY_FREETYPE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/freetype/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
        ${THIRDPARTY_FREETYPE_DIR}src/autofit/autofit.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftbase.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftbbox.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftbdf.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftbitmap.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftcid.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftdebug.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftfstype.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftgasp.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftglyph.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftgxval.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftinit.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftmm.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftotval.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftpatent.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftpfr.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftstroke.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftsynth.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftsystem.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/fttype1.c
        ${THIRDPARTY_FREETYPE_DIR}src/base/ftwinfnt.c
        ${THIRDPARTY_FREETYPE_DIR}src/bdf/bdf.c
        ${THIRDPARTY_FREETYPE_DIR}src/bzip2/ftbzip2.c
        ${THIRDPARTY_FREETYPE_DIR}src/cache/ftcache.c
        ${THIRDPARTY_FREETYPE_DIR}src/cff/cff.c
        ${THIRDPARTY_FREETYPE_DIR}src/cid/type1cid.c
        ${THIRDPARTY_FREETYPE_DIR}src/gxvalid/gxvalid.c
        ${THIRDPARTY_FREETYPE_DIR}src/gzip/ftgzip.c
        ${THIRDPARTY_FREETYPE_DIR}src/lzw/ftlzw.c
        ${THIRDPARTY_FREETYPE_DIR}src/otvalid/otvalid.c
        ${THIRDPARTY_FREETYPE_DIR}src/pcf/pcf.c
        ${THIRDPARTY_FREETYPE_DIR}src/pfr/pfr.c
        ${THIRDPARTY_FREETYPE_DIR}src/psaux/psaux.c
        ${THIRDPARTY_FREETYPE_DIR}src/pshinter/pshinter.c
        ${THIRDPARTY_FREETYPE_DIR}src/psnames/psnames.c
        ${THIRDPARTY_FREETYPE_DIR}src/raster/raster.c
        ${THIRDPARTY_FREETYPE_DIR}src/sdf/sdf.c
        ${THIRDPARTY_FREETYPE_DIR}src/svg/svg.c
        ${THIRDPARTY_FREETYPE_DIR}src/smooth/smooth.c
        ${THIRDPARTY_FREETYPE_DIR}src/truetype/truetype.c
        ${THIRDPARTY_FREETYPE_DIR}src/type1/type1.c
        ${THIRDPARTY_FREETYPE_DIR}src/type42/type42.c
        ${THIRDPARTY_FREETYPE_DIR}src/winfonts/winfnt.c
		${THIRDPARTY_FREETYPE_DIR}src/sfnt/sfnt.c
)

if (BROTLI)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		FT_CONFIG_OPTION_USE_BROTLI
	)
endif()

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_FREETYPE_DIR}include/
)

target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
	FT2_BUILD_LIBRARY
	FT_CONFIG_OPTION_USE_PNG
	FT_CONFIG_OPTION_SYSTEM_ZLIB
)

if (BUILTIN_LIBPNG)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/libpng/
	)
endif()

file(GLOB FREETYPE_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${FREETYPE_SOURCES}
)
