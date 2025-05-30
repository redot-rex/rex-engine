add_library(rex_module_${MODULE_NAME} STATIC)
set(MODULE_IS_ENABLED ON PARENT_SCOPE)

target_link_libraries(rex_module_${MODULE_NAME} PUBLIC
	rex_module_jsonrpc
	rex_module_websocket
)

add_dependencies(rex_module_${MODULE_NAME}
	rex_module_jsonrpc
	rex_module_websocket
)

file(GLOB GDSCRIPT_SOURCES *.cpp)
target_sources(rex_module_${MODULE_NAME} PRIVATE
	${GDSCRIPT_SOURCES}
)

if (EDITOR_BUILD)
	file(GLOB GDSCRIPT_EDITOR_SOURCES editor/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GDSCRIPT_EDITOR_SOURCES}
	)
	if("jsonrpc" IN_LIST ENABLED_MODULES AND "websocket" IN_LIST ENABLED_MODULES)
		file(GLOB GDSCRIPT_LANGUAGE_SERVER_SOURCES language_server/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${GDSCRIPT_LANGUAGE_SERVER_SOURCES}
		)
	else()
		message(WARNING "NO LSP")
		target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
			GDSCIPT_NO_LSP
		)
		target_compile_definitions(rex_engine PUBLIC
			GDSCIPT_NO_LSP
		)
	endif()
endif()

if (TESTS)
	target_compile_definitions(rex_module_${MODULE_NAME} PRIVATE
		TESTS_ENABLED
	)
	file(GLOB GDSCRIPT_TESTS_SOURCES tests/*.cpp)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${GDSCRIPT_TESTS_SOURCES}
	)
endif()

