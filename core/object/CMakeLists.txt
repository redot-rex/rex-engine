add_custom_command(
	OUTPUT ${CMAKE_SOURCE_DIR}/core/object/gdvirtual.gen.inc
	COMMAND "${_Python3_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/generators/rex_make_virtuals.py"
	"${CMAKE_SOURCE_DIR}/core/object/gdvirtual.gen.inc"
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMENT "Generating gdvirtual.gen.inc ..."
)
add_custom_target(gdvirtual DEPENDS
	${CMAKE_SOURCE_DIR}/core/object/gdvirtual.gen.inc
)

add_dependencies(rex_core gdvirtual)

file(GLOB_RECURSE CORE_OBJECT_SOURCES *.cpp)

target_sources(rex_core PRIVATE ${CORE_OBJECT_SOURCES})
