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

target_link_libraries(rex_engine PUBLIC rex_main)
