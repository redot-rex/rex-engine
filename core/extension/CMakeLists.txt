add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/core/extension/ext_wrappers.gen.inc
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_make_wrappers.py"
	"${CMAKE_SOURCE_DIR}/core/extension/ext_wrappers.gen.inc"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating ext_wrappers.gen.inc ..."
)
add_custom_target(ext_wrappers DEPENDS
	${CMAKE_SOURCE_DIR}/core/extension/ext_wrappers.gen.inc
)

add_dependencies(rex_core ext_wrappers)

add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/core/extension/gdextension_interface_dump.gen.h
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_make_interface_dumper.py"
	"${CMAKE_SOURCE_DIR}/core/extension/gdextension_interface_dump.gen.h"
	"${CMAKE_SOURCE_DIR}/core/extension/gdextension_interface.h"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating gdextension_interface_dump.gen.h ..."
)
add_custom_target(gdextension_interface_dump DEPENDS
	${CMAKE_SOURCE_DIR}/core/extension/gdextension_interface_dump.gen.h
)

add_dependencies(rex_core gdextension_interface_dump)

file(GLOB_RECURSE CORE_EXTENSION_SOURCES *.cpp)
target_sources(rex_core PRIVATE ${CORE_EXTENSION_SOURCES})
