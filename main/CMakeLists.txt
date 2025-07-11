file(GLOB_RECURSE MAIN_SOURCES *.cpp)

add_library(rex_main STATIC
	${MAIN_SOURCES}
)

clone_library(rex_engine rex_main)

if (STEAMAPI AND EDITOR_BUILD)
	target_compile_definitions(rex_main PRIVATE
		STEAMAPI_ENABLED
	)
endif()

if (REX_TESTS)
	target_compile_definitions(rex_main PRIVATE
		TESTS_ENABLED
	)
endif()

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/main/splash.gen.h
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_main_builders.py" make_splash
	"${CMAKE_SOURCE_DIR}/main/splash.gen.h" "${CMAKE_SOURCE_DIR}/main/splash.png"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating splash.gen.h ..."
)
add_custom_target(splash DEPENDS
	${CMAKE_SOURCE_DIR}/main/splash.gen.h
)

add_dependencies(rex_main splash)

if (EDITOR_BUILD AND NOT NO_EDITOR_SPLASH)
	add_custom_command(
		OUTPUT ${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h
		COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_main_builders.py" make_splash_editor
		"${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h" "${CMAKE_SOURCE_DIR}/main/splash_editor.png"
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		COMMENT "Generating splash_editor.gen.h ..."
	)
	add_custom_target(splash_editor DEPENDS
		${CMAKE_SOURCE_DIR}/main/splash_editor.gen.h
	)

	add_dependencies(rex_main splash_editor)
endif()

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/main/app_icon.gen.h
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_main_builders.py" make_app_icon
	"${CMAKE_SOURCE_DIR}/main/app_icon.gen.h" "${CMAKE_SOURCE_DIR}/main/app_icon.png"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating app_icon.gen.h ..."
)
add_custom_target(app_icon DEPENDS
	${CMAKE_SOURCE_DIR}/main/app_icon.gen.h
)

add_dependencies(rex_main app_icon)

target_link_libraries(rex_engine PUBLIC rex_main)
