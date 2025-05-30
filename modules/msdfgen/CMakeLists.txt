add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_freetype
)

if (BUILTIN_MSDFGEN)
	set(THIRDPARTY_MSDFGEN_DIR ${CMAKE_SOURCE_DIR}/thirdparty/msdfgen/)
	add_library(msdfgen_builtin STATIC
        ${THIRDPARTY_MSDFGEN_DIR}core/Contour.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/DistanceMapping.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/EdgeHolder.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/MSDFErrorCorrection.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/Projection.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/Scanline.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/Shape.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/contour-combiners.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/edge-coloring.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/edge-segments.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/edge-selectors.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/equation-solver.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/msdf-error-correction.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/msdfgen.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/rasterization.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/render-sdf.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/sdf-error-estimation.cpp
        ${THIRDPARTY_MSDFGEN_DIR}core/shape-description.cpp
	)
	target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
		MSDFGEN_PUBLIC=
	)
	target_include_directories(rex_module_${MODULE_NAME} INTERFACE
		${CMAKE_SOURCE_DIR}/thirdparty/freetype/include
		${CMAKE_SOURCE_DIR}/thirdparty/msdfgen
		${CMAKE_SOURCE_DIR}/thirdparty/nanosvg
	)
	clone_library(rex_module_${MODULE_NAME} msdfgen_builtin)
	disable_warnings_for_target(msdfgen_builtin)
	list(APPEND BUILTIN_LIBRARIES msdfgen_builtin)

endif()

file(GLOB MSDFGEN_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${MSDFGEN_SOURCES}
)
set(BUILTIN_LIBRARIES ${BUILTIN_LIBRARIES} PARENT_SCOPE)
