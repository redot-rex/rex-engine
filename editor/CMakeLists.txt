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

target_include_directories(rex_editor PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_editor PRIVATE ${ENGINE_DEFINITIONS})

add_subdirectory(debugger)
add_subdirectory(export)
add_subdirectory(gui)
# TODO: Need python script to build icon files.
add_subdirectory(import)
add_subdirectory(plugins)
add_subdirectory(project_manager)
add_subdirectory(themes)

#target_link_libraries(rex_engine PUBLIC rex_editor)
