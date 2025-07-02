file(GLOB ICON_SOURCES *.svg)

list(JOIN ICON_SOURCES " " ICON_FILES_ARGS)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/scene/theme/default_theme_icons.gen.h
	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/generators/rex_default_theme_icon_builders.py make_default_theme_icons_action ${CMAKE_SOURCE_DIR}/scene/theme/default_theme_icons.gen.h ${ICON_FILES_ARGS}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating default theme icon actions files..."
	DEPENDS ${ICON_SOURCES}
)
add_custom_target(default_theme_icon_action_header DEPENDS
	${CMAKE_SOURCE_DIR}/scene/theme/default_theme_icons.gen.h
)

add_dependencies(rex_scene default_theme_icon_action_header)
