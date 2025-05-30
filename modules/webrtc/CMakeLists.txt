add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

# TODO: Web stuff

file(GLOB WEBRTC_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${WEBRTC_SOURCES}
)
