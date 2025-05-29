add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})
file(GLOB BCDEC_SOURCES *.cpp)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${BCDEC_SOURCES}
)
