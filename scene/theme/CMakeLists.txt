add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/scene/theme/default_font.gen.h
	COMMAND ${_Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/rex_default_theme_builders.py make_fonts_header ${CMAKE_SOURCE_DIR}/scene/theme/default_font.gen.h ${CMAKE_SOURCE_DIR}/thirdparty/fonts/OpenSans_SemiBold.woff2
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating default fonts header..."
)
add_custom_target(default_fonts_header DEPENDS
	${CMAKE_SOURCE_DIR}/scene/theme/default_font.gen.h
)

add_subdirectory(icons)

add_dependencies(rex_scene default_fonts_header)

file(GLOB_RECURSE SCENE_THEME_SOURCES *.cpp)
target_sources(rex_scene PRIVATE ${SCENE_THEME_SOURCES})
