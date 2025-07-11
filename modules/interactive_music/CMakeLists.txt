file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	AudioStreamPlaylist
	AudioStreamPlaybackPlaylist
	AudioStreamInteractive
	AudioStreamPlaybackInteractive
	AudioStreamSynchronized
	AudioStreamPlaybackSynchronized
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

file(GLOB INTERACTIVE_MUSIC_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${INTERACTIVE_MUSIC_SOURCES}
)
if (EDITOR_BUILD)
	file(GLOB INTERACTIVE_MUSIC_EDITOR_SOURCES editor/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${INTERACTIVE_MUSIC_EDITOR_SOURCES}
	)
endif()
