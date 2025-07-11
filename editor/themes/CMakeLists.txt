file(GLOB FONT_LIST
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.ttf
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.otf
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.woff
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.woff2
)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h
	COMMAND ${_Python3_EXECUTABLE}
	ARGS ${CMAKE_SOURCE_DIR}/generators/rex_editor_theme_builders.py make_fonts_header
	${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h ${FONT_LIST}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating Builtin Fonts Header..."
)
add_custom_target(builtin_fonts DEPENDS ${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h)
add_dependencies(rex_editor builtin_fonts)

file(GLOB_RECURSE EDITOR_THEMES_SOURCES *.cpp)
target_sources(rex_editor PRIVATE ${EDITOR_THEMES_SOURCES})
