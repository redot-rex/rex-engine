file(GLOB EDITOR_SOURCES *.cpp)

file(GLOB EDITOR_PLATFORM_ANDROID_SOURCES ${CMAKE_SOURCE_DIR}/platform/android/export/*.cpp)
file(GLOB EDITOR_PLATFORM_IOS_SOURCES ${CMAKE_SOURCE_DIR}/platform/ios/export/*.cpp)
file(GLOB EDITOR_PLATFORM_LINUXBSD_SOURCES ${CMAKE_SOURCE_DIR}/platform/linuxbsd/export/*.cpp)
file(GLOB EDITOR_PLATFORM_MACOS_SOURCES ${CMAKE_SOURCE_DIR}/platform/macos/export/*.cpp)
file(GLOB EDITOR_PLATFORM_WEB_SOURCES ${CMAKE_SOURCE_DIR}/platform/web/export/*.cpp)
file(GLOB EDITOR_PLATFORM_WINDOWS_SOURCES ${CMAKE_SOURCE_DIR}/platform/windows/export/*.cpp)


add_library(rex_editor STATIC
	${EDITOR_SOURCES}
	${EDITOR_PLATFORM_ANDROID_SOURCES}
	${EDITOR_PLATFORM_IOS_SOURCES}
	${EDITOR_PLATFORM_LINUXBSD_SOURCES}
	${EDITOR_PLATFORM_MACOS_SOURCES}
	${EDITOR_PLATFORM_WEB_SOURCES}
	${EDITOR_PLATFORM_WINDOWS_SOURCES}
)

clone_library(rex_engine rex_editor)

file(GLOB DOC_PATHS ${CMAKE_SOURCE_DIR}/doc/classes/*)

list(JOIN DOC_PATHS " " DOC_FILES_ARGS)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_editor_builders.py" doc_data_class_path_builder
	"${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h" ${DOC_FILES_ARGS}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating doc_data_class_path.gen.h ..."
)
add_custom_target(doc_data_class_path DEPENDS
	${CMAKE_SOURCE_DIR}/editor/doc_data_class_path.gen.h
)
add_dependencies(rex_editor doc_data_class_path)


add_subdirectory(debugger)
add_subdirectory(export)
add_subdirectory(gui)
# TODO: Need python script to build icon files.
add_subdirectory(import)
add_subdirectory(plugins)
add_subdirectory(project_manager)
add_subdirectory(themes)

#target_link_libraries(rex_engine PUBLIC rex_editor)
