# TODO: Scons runs a python script here

function (add_wayland_header outfile src)
	if (USE_SOWRAP)
		execute_process(
			COMMAND /bin/sh -c "${WAYLAND_SCANNER} -c client-header < ${src} | sed 's:wayland-client-core.h:../dynwrappers/wayland-client-core-so_wrap.h:' > ${outfile}"
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
	else()
		execute_process(
			COMMAND /bin/sh -c "${WAYLAND_SCANNER} -c client-header < ${src} > ${outfile}"
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
	endif()
endfunction()

function (add_wayland_code outfile src)
	if (USE_SOWRAP)
		execute_process(
			COMMAND /bin/sh -c "${WAYLAND_SCANNER} -c private-code < ${src} | sed 's:wayland-util.h:../dynwrappers/wayland-client-core-so_wrap.h:' > ${outfile}"
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
	else()
		execute_process(
			COMMAND /bin/sh -c "${WAYLAND_SCANNER} -c private-code < ${src} > ${outfile}"
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		)
	endif()
endfunction()


file(MAKE_DIRECTORY protocol)

add_wayland_header(
	protocol/wayland.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland/protocol/wayland.xml
)
add_wayland_code(
	protocol/wayland.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland/protocol/wayland.xml
)
add_wayland_header(
	protocol/viewporter.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/stable/viewporter/viewporter.xml
)
add_wayland_code(
	protocol/viewporter.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/stable/viewporter/viewporter.xml
)
add_wayland_header(
	protocol/fractional_scale.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/fractional-scale/fractional-scale-v1.xml
)
add_wayland_code(
	protocol/fractional_scale.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/fractional-scale/fractional-scale-v1.xml
)
add_wayland_header(
	protocol/xdg_shell.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/stable/xdg-shell/xdg-shell.xml
)
add_wayland_code(
	protocol/xdg_shell.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/stable/xdg-shell/xdg-shell.xml
)
add_wayland_header(
	protocol/xdg_decoration.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
)
add_wayland_code(
	protocol/xdg_decoration.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
)
add_wayland_header(
	protocol/xdg_activation.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/xdg-activation/xdg-activation-v1.xml
)
add_wayland_code(
	protocol/xdg_activation.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/xdg-activation/xdg-activation-v1.xml
)
add_wayland_header(
	protocol/relative_pointer.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml
)
add_wayland_code(
	protocol/relative_pointer.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/relative-pointer/relative-pointer-unstable-v1.xml
)
add_wayland_header(
	protocol/pointer_constraints.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml
)
add_wayland_code(
	protocol/pointer_constraints.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/pointer-constraints/pointer-constraints-unstable-v1.xml
)
add_wayland_header(
	protocol/pointer_gestures.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/pointer-gestures/pointer-gestures-unstable-v1.xml
)
add_wayland_code(
	protocol/pointer_gestures.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/pointer-gestures/pointer-gestures-unstable-v1.xml
)
add_wayland_header(
	protocol/primary_selection.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/primary-selection/primary-selection-unstable-v1.xml
)
add_wayland_code(
	protocol/primary_selection.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/primary-selection/primary-selection-unstable-v1.xml
)
add_wayland_header(
	protocol/idle_inhibit.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml
)
add_wayland_code(
	protocol/idle_inhibit.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/idle-inhibit/idle-inhibit-unstable-v1.xml
)
add_wayland_header(
	protocol/tablet.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/tablet/tablet-unstable-v2.xml
)
add_wayland_code(
	protocol/tablet.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/tablet/tablet-unstable-v2.xml
)
add_wayland_header(
	protocol/text_input.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/text-input/text-input-unstable-v3.xml
)
add_wayland_code(
	protocol/text_input.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/text-input/text-input-unstable-v3.xml
)
add_wayland_header(
	protocol/xdg_foreign_v1.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-foreign/xdg-foreign-unstable-v1.xml
)
add_wayland_code(
	protocol/xdg_foreign_v1.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-foreign/xdg-foreign-unstable-v1.xml
)
add_wayland_header(
	protocol/xdg_foreign_v2.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-foreign/xdg-foreign-unstable-v2.xml
)
add_wayland_code(
	protocol/xdg_foreign_v2.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/unstable/xdg-foreign/xdg-foreign-unstable-v2.xml
)
add_wayland_header(
	protocol/xdg_system_bell.gen.h
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/xdg-system-bell/xdg-system-bell-v1.xml
)
add_wayland_code(
	protocol/xdg_system_bell.gen.c
	${CMAKE_SOURCE_DIR}/thirdparty/wayland-protocols/staging/xdg-system-bell/xdg-system-bell-v1.xml
)

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
