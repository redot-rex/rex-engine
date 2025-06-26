file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	SceneReplicationConfig
	SceneMultiplayer
	MultiplayerSpawner
	MultiplayerSynchronizer
	OfflineMultiplayerPeer
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

file(GLOB MULTIPLAYER_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${MULTIPLAYER_SOURCES}
)
if (EDITOR_BUILD)
	file(GLOB MULTIPLAYER_EDITOR_SOURCES editor/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${MULTIPLAYER_EDITOR_SOURCES}
	)
endif()

if (REX_TESTS)
	target_compile_definitions(rex_moudle_${MODULE_NAME} PRIVATE
		TESTS_ENABLED
	)
	file(GLOB MULTIPLAYER_TESTS_SOURCES test/*.cpp)
	target_sources(rex_module_${MODULE_NAME}
		${MULTIPLAYER_TESTS_SOURCES}
	)
	if (DISABLE_EXCEPTIONS)
		target_compile_definitions(rex_module_${MODULE_NAME}
			DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
		)
	endif()
endif()
