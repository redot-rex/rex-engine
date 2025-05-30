add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

set(THIRDPARTY_ETCPAK_DIR ${CMAKE_SOURCE_DIR}/thirdparty/etcpak/)
add_library(etcpak OBJECT
    ${THIRDPARTY_ETCPAK_DIR}DecodeRGB.cpp
    ${THIRDPARTY_ETCPAK_DIR}Dither.cpp
    ${THIRDPARTY_ETCPAK_DIR}ProcessDxtc.cpp
    ${THIRDPARTY_ETCPAK_DIR}ProcessRGB.cpp
    ${THIRDPARTY_ETCPAK_DIR}Tables.cpp
)
target_include_directories(rex_module_${MODULE_NAME} INTERFACE
	${THIRDPARTY_ETCPAK_DIR}
)

clone_library(rex_module_${MODULE_NAME} etcpak)
disable_warnings_for_target(etcpak)
target_link_libraries(rex_module_${MODULE_NAME} PUBLIC etcpak)

file(GLOB_RECURSE ETCPAK_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${ETCPAK_SOURCES}
)
