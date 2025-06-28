set(CONTROLLER_DATABASES
	${CMAKE_CURRENT_SOURCE_DIR}/gamecontrollerdb.txt
    ${CMAKE_CURRENT_SOURCE_DIR}/godotcontrollerdb.txt
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/default_controller_mappings.gen.cpp
	COMMAND ${_Python3_EXECUTABLE}
	ARGS ${CMAKE_SOURCE_DIR}/generators/rex_input_builders.py make_default_controller_mappings
	${CMAKE_CURRENT_SOURCE_DIR}/default_controller_mappings.gen.cpp ${CONTROLLER_DATABASES}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating controller mappings..."
)

add_custom_target(default_controller_mappings DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/default_controller_mappings.gen.cpp)
add_dependencies(rex_core default_controller_mappings)
target_sources(rex_core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/default_controller_mappings.gen.cpp)

file(GLOB_RECURSE CORE_INPUT_SOURCES *.cpp)
target_sources(rex_core PRIVATE ${CORE_INPUT_SOURCES})
