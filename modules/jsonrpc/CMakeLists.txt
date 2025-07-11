add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

file(GLOB JSONRPC_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${JSONRPC_SOURCES}
)
if (REX_TESTS)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		TESTS_ENABLED
	)
	file(GLOB JSONRPC_TESTS_SOURCES tests/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${JSONRPC_TESTS_SOURCES}
	)
	if (DISABLE_EXCEPTIONS)
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS
		)
	endif()
endif()
