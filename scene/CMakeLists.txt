file(GLOB SCENE_SOURCES *.cpp)

add_library(rex_scene STATIC ${SCENE_SOURCES})

target_include_directories(rex_scene PRIVATE "$<TARGET_PROPERTY:rex_engine,INTERFACE_INCLUDE_DIRECTORIES>")

get_target_property(ENGINE_DEFINITIONS rex_engine INTERFACE_COMPILE_DEFINITIONS)

target_compile_definitions(rex_scene PRIVATE ${ENGINE_DEFINITIONS})

add_subdirectory(main)
add_subdirectory(gui)
if (NOT DISABLE_3D)
    add_subdirectory(3d)
endif()
add_subdirectory(2d)
add_subdirectory(animation)
add_subdirectory(audio)
add_subdirectory(resources)
add_subdirectory(debugger)
add_subdirectory(theme)

#target_link_libraries(rex_engine PUBLIC rex_scene)
