add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

set(THIRDPARTY_THORVG_DIR ${CMAKE_SOURCE_DIR}/thirdparty/thorvg/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
    # common
    ${THIRDPARTY_THORVG_DIR}src/common/tvgCompressor.cpp
    ${THIRDPARTY_THORVG_DIR}src/common/tvgMath.cpp
    ${THIRDPARTY_THORVG_DIR}src/common/tvgStr.cpp
    # SVG parser
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgSvgCssStyle.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgSvgLoader.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgSvgPath.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgSvgSceneBuilder.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgSvgUtil.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/svg/tvgXmlParser.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/raw/tvgRawLoader.cpp
    # image loaders
    ${THIRDPARTY_THORVG_DIR}src/loaders/external_png/tvgPngLoader.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/jpg/tvgJpgd.cpp
    ${THIRDPARTY_THORVG_DIR}src/loaders/jpg/tvgJpgLoader.cpp
    # renderer common
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgAccessor.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgCanvas.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgFill.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgInitializer.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgLoader.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgPaint.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgPicture.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgRender.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgScene.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgShape.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgSwCanvas.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgTaskScheduler.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/tvgText.cpp
    # renderer sw_engine
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwFill.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwImage.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwMath.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwMemPool.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwPostEffect.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwRaster.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwRenderer.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwRle.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwShape.cpp
    ${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine/tvgSwStroke.cpp
)
if ("webp" IN_LIST ENABLED_MODULES)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_THORVG_DIR}src/loaders/external_webp/tvgWebpLoader.cpp
	)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		THORVG_WEBP_LOADER_SUPPORT
	)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${THIRDPARTY_THORVG_DIR}src/loaders/external_webp/
	)
	if (BUILTIN_LIBWEBP)
        target_include_directories(rex_module_${MODULE_NAME} PRIVATE
			${CMAKE_SOURCE_DIR}thirdparty/libwebp/src/
		)
	endif()
endif()

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_THORVG_DIR}inc/
	${THIRDPARTY_THORVG_DIR}src/common
	${THIRDPARTY_THORVG_DIR}src/loaders/svg
	${THIRDPARTY_THORVG_DIR}src/renderer
	${THIRDPARTY_THORVG_DIR}src/renderer/sw_engine
	${THIRDPARTY_THORVG_DIR}src/loaders/raw
	${THIRDPARTY_THORVG_DIR}src/loaders/external_png
	${THIRDPARTY_THORVG_DIR}src/loaders/jpg
)
target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
	TVG_STATIC
	THORVG_FILE_IO_SUPPORT
)
if (BUILTIN_LIBPNG)
	target_include_directories(rex_module_${MODULE_NAME} PRIVATE
		${CMAKE_SOURCE_DIR}/thirdparty/libpng/
	)
endif()

file(GLOB SVG_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${SVG_SOURCES}
)
