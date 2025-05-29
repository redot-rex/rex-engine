add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/thirdparty/noise/
)

file(GLOB NOISE_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${NOISE_SOURCES}
)
if (EDITOR_BUILD)
	file(GLOB NOISE_EDITOR_SOURCES editor/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${NOISE_EDITOR_SOURCES}
	)
endif()
