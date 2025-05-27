# TODO: Scons runs a python script here
target_sources(rex_engine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/wayland.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/viewporter.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/fractional_scale.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_shell.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_system_bell.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_foreign_v1.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_foreign_v2.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_decoration.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/xdg_activation.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/relative_pointer.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/pointer_constraints.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/pointer_gestures.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/primary_selection.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/idle_inhibit.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/tablet.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/protocol/text_input.gen.c
	${CMAKE_CURRENT_SOURCE_DIR}/display_server_wayland.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wayland_thread.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/key_mapping_xkb.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/detect_prime_egl.cpp
)

		target_include_directories(rex_engine PUBLIC
			${CMAKE_SOURCE_DIR}/thirdparty/linuxbsd_headers/wayland/
		)

if (USE_SOWRAP)
	target_sources(rex_engine PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/wayland-cursor-so_wrap.c
		${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/wayland-client-core-so_wrap.c
		${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/wayland-egl-core-so_wrap.c
	)

	if (LIBDECOR)
		target_compile_definitions(rex_engine PUBLIC
			LIBDECOR_ENABLED
		)
		target_include_directories(rex_engine PUBLIC
			${CMAKE_SOURCE_DIR}/thirdparty/linuxbsd_headers/libdecor-0/
		)
		target_sources(rex_engine PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/dynwrappers/libdecor-so_wrap.c
		)
	endif()

	if (VULKAN)
		target_sources(rex_engine PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/rendering_context_driver_vulkan_wayland.cpp
		)
	endif()

	if (OPENGL3)
		target_sources(rex_engine PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/egl_manager_wayland.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/egl_manager_wayland_gles.cpp
		)
	endif()
endif()
