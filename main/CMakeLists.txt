file(GLOB_RECURSE MAIN_SOURCES *.cpp)

add_library(rex_main STATIC
	${MAIN_SOURCES}
)

target_include_directories(rex_main PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_main PRIVATE ${ENGINE_DEFINITIONS})

target_link_libraries(rex_engine PUBLIC rex_main)
