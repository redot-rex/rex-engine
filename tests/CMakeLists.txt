set(TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/)

file(GLOB TESTS_SOURCES *.cpp)

add_library(rex_tests STATIC
	${TESTS_SOURCES}
)

clone_library(rex_engine rex_servers)

if (REX_PLATFORM STREQUAL "windows")
	target_compile_definitions(rex_tests PRIVATE
		DOCTEST_THREAD_LOCAL=
	)
endif()

if (DISABLE_EXCEPTIONS)
	target_compile_definitions(rex_tests PRIVATE
		DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
	)
endif()

target_link_libraries(rex_engine PUBLIC rex_tests)
