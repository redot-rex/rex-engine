file(RELATIVE_PATH CURRENT_RELATIVE_PATH "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

set(MODULE_DOC_CLASSES
	OpenXRInterface
	OpenXRAction
	OpenXRActionSet
	OpenXRActionMap
	OpenXRAPIExtension
	OpenXRExtensionWrapper
	OpenXRExtensionWrapperExtension
	OpenXRFutureResult
	OpenXRFutureExtension
	OpenXRInteractionProfile
	OpenXRInteractionProfileMetadata
	OpenXRIPBinding
	OpenXRHand
	OpenXRVisibilityMask
	OpenXRCompositionLayer
	OpenXRCompositionLayerQuad
	OpenXRCompositionLayerCylinder
	OpenXRCompositionLayerEquirect
	OpenXRBindingModifier
	OpenXRIPBindingModifier
	OpenXRActionBindingModifier
	OpenXRAnalogThresholdModifier
	OpenXRDpadBindingModifier
	OpenXRInteractionProfileEditorBase
	OpenXRInteractionProfileEditor
	OpenXRBindingModifierEditor
	OpenXRHapticBase
	OpenXRHapticVibration
)
set(MODULE_DOC_CLASS_PATH "${CURRENT_RELATIVE_PATH}/doc_classes")

foreach(DOC_CLASS ${MODULE_DOC_CLASSES})
	list(APPEND DOC_CLASS_PATHS "${DOC_CLASS},${MODULE_DOC_CLASS_PATH}")
endforeach()

set(DOC_CLASS_PATHS ${DOC_CLASS_PATHS} PARENT_SCOPE)

if (REX_PLATFORM MATCHES "linuxbsd|android|macos|windows" AND NOT DISABLE_XR)
	add_library(rex_module_${MODULE_NAME} STATIC)
	set(MODULE_IS_ENABLED ON PARENT_SCOPE)

	if (REX_PLATFORM STREQUAL "android")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			XR_OS_ANDROID
			XR_USE_PLATFORM_ANDROID
			JSON_USE_EXCEPTION=0
		)
	elseif(REX_PLATFORM STREQUAL "linuxbsd")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			XR_OS_LINUX
		)
		if (X11)
			target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
				XR_USE_PLATFORM_XLIB
			)
		endif()
		if (WAYLAND AND OPENGL3)
			target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
				XR_USE_PLATFORM_EGL
			)
		endif()
	endif()

	# FIXME: Review what needs to be set for Android and macOS.
	# FreeBSD uses non-standard getenv functions.

	if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			HAVE_SECURE_GETENV
		)
	elseif(REX_PLATFORM STREQUAL "windows")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			XR_OS_WINDOWS
			NOMINMAX
			XR_USE_PLATFORM_WIN32
		)
	elseif(REX_PLATFORM STREQUAL "macos")
		target_compile_definitions(rex_module_${MODULE_NAME} INTERFACE
			XR_OS_APPLE
		)
	endif()
	if (BUILTIN_OPENXR)
		set(THIRDPARTY_OPENXR_DIR ${CMAKE_SOURCE_DIR}/thirdparty/openxr/)
		add_library(openxr OBJECT
			${THIRDPARTY_OPENXR_DIR}src/external/jsoncpp/src/lib_json/json_reader.cpp
			${THIRDPARTY_OPENXR_DIR}src/external/jsoncpp/src/lib_json/json_value.cpp
			${THIRDPARTY_OPENXR_DIR}src/external/jsoncpp/src/lib_json/json_writer.cpp
		)

		if (NOT REX_PLATFORM STREQUAL "android")
			target_sources(openxr PRIVATE
				${THIRDPARTY_OPENXR_DIR}src/xr_generated_dispatch_table_core.c
				${THIRDPARTY_OPENXR_DIR}src/common/filesystem_utils.cpp
				${THIRDPARTY_OPENXR_DIR}src/common/object_info.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/api_layer_interface.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/loader_core.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/loader_instance.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/loader_logger_recorders.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/loader_logger.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/manifest_file.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/runtime_interface.cpp
				${THIRDPARTY_OPENXR_DIR}src/loader/xr_generated_loader.cpp
			)
		endif()

		target_include_directories(rex_module_${MODULE_NAME} INTERFACE
            ${THIRDPARTY_OPENXR_DIR}
            ${THIRDPARTY_OPENXR_DIR}include/
            ${THIRDPARTY_OPENXR_DIR}src/
            ${THIRDPARTY_OPENXR_DIR}src/common/
            ${THIRDPARTY_OPENXR_DIR}src/external/jsoncpp/include/
		)

		clone_library(rex_module_${MODULE_NAME} openxr)
		disable_warnings_for_target(openxr)

		target_compile_definitions(openxr PRIVATE
			DISABLE_STD_FILESYSTEM
		)
		if (DISABLE_EXCEPTIONS)
			target_compile_definitions(openxr PRIVATE
				XRLOADER_DISABLE_EXCEPTION_HANDLING
				JSON_USE_EXCEPTION=0
			)
		endif()
		target_include_directories(openxr PRIVATE
			${THIRDPARTY_OPENXR_DIR}src/loader/
		)

		target_link_libraries(rex_module_${MODULE_NAME} PUBLIC openxr)

	endif()

	file(GLOB OPENXR_SOURCES
		*.cpp
		action_map/*.cpp
		scene/*.cpp
	)
	target_sources(rex_module_${MODULE_NAME} PRIVATE
		${OPENXR_SOURCES}
	)
	add_subdirectory(extensions)
	if (EDITOR_BUILD)
		file(GLOB OPENXR_EDITOR_SOURCES editor/*.cpp)
		target_sources(rex_module_${MODULE_NAME} PRIVATE
			${OPENXR_EDITOR_SOURCES}
		)
	endif()
endif()
