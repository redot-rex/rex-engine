add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)
disable_warnings_for_target(rex_module_${MODULE_NAME})

set(THIRDPARTY_ETCPAK_DIR ${CMAKE_SOURCE_DIR}/thirdparty/etcpak/)
target_sources(rex_module_${MODULE_NAME} PRIVATE
    ${THIRDPARTY_ETCPAK_DIR}DecodeRGB.cpp
    ${THIRDPARTY_ETCPAK_DIR}Dither.cpp
    ${THIRDPARTY_ETCPAK_DIR}ProcessDxtc.cpp
    ${THIRDPARTY_ETCPAK_DIR}ProcessRGB.cpp
    ${THIRDPARTY_ETCPAK_DIR}Tables.cpp
)
target_include_directories(rex_module_${MODULE_NAME} PRIVATE
	${THIRDPARTY_ETCPAK_DIR}
)

file(GLOB_RECURSE ETCPAK_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${ETCPAK_SOURCES}
)
