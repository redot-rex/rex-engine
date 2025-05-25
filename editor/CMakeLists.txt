
file(GLOB_RECURSE EDITOR_SOURCES *.cpp *.h)

file(GLOB ICON_SOURCES
	${CMAKE_SOURCE_DIR}/editor/icons/*.png
	${CMAKE_SOURCE_DIR}/editor/icons/*.svg
)

file(GLOB FONT_SOURCES
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.ttf
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.otf
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.woff
	${CMAKE_SOURCE_DIR}/thirdparty/fonts/*.woff2
)

add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/editor/icons/editor_icons_builders.py ${ICON_SOURCES}
  DEPENDS ${ICON_SOURCES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/editor/icons
  COMMENT "Generating editor_icons.gen.h"
)

add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/editor/themes/editor_theme_builders.py ${FONT_SOURCES}
  DEPENDS ${FONT_SOURCES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/editor/themes
  COMMENT "Generating builtin_fonts.gen.h"
)

add_custom_target(generate_editor_icons DEPENDS ${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h)
add_custom_target(generate_editor_fonts DEPENDS ${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h)

target_sources(rex_engine PUBLIC
	${EDITOR_SOURCES}
	${CMAKE_SOURCE_DIR}/editor/themes/editor_icons.gen.h
	${CMAKE_SOURCE_DIR}/editor/themes/builtin_fonts.gen.h
)


