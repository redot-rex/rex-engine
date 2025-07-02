target_sources(rex_engine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/display_server_x11.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/key_mapping_x11.cpp
)

if (USE_SOWRAP)
	target_sources(rex_engine PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xlib-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xcursor-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xinerama-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xinput2-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xrandr-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xrender-so_wrap.c
            ${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/xext-so_wrap.c
    )
endif()
if (VULKAN)
	target_sources(rex_engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/rendering_context_driver_vulkan_x11.cpp
	)
endif()

if (OPENGL3)
	target_compile_definitions(rex_engine PRIVATE
		GLAD_GLX_NO_X11
	)
	target_sources(rex_engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/gl_manager_x11_egl.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/gl_manager_x11.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/detect_prime_x11.cpp
		${CMAKE_SOURCE_DIR}/thirdparty/glad/glx.c
    )
endif()
