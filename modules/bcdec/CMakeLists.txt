file(GLOB BCDEC_SOURCES *.cpp)

target_sources(rex_module_${MODULE_NAME} PRIVATE
	${BCDEC_SOURCES}
)
