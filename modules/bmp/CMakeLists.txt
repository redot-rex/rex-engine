file(GLOB BMP_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${BMP_SOURCES}
)
