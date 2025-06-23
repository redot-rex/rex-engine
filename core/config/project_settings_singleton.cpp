/**************************************************************************/
/*  project_settings_singleton.cpp                                        */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             REDOT ENGINE                               */
/*                        https://redotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2024-present Redot Engine contributors                   */
/*                                          (see REDOT_AUTHORS.md)        */
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "project_settings.h"

#include "core/core_bind.h"
#include "core/io/config_file.h"

/*
 * Returns the global singleton instance of ProjectSettings.
 *
 * @return - Pointer to ProjectSettings singleton.
 */
ProjectSettings *ProjectSettings::get_singleton() {
	return singleton;
}

/*
 * Registers project settings with full metadata.
 *
 * @param p_info - The property information.
 *
 * @param p_default - The default value for the setting.
 *
 * @param p_restart_if_changed - Project requires restart if setting is
 *                               changed, if True.
 *
 * @param p_ignore_value_in_docs - Ignore this setting in documentation, if
 *                                 True.
 *
 * @param p_basic - The setting is a basic user-facing property, if True.
 *
 * @param p_internal - The setting is for internal use only, if True.
 *
 * @return - The registered setting's value.
 */
Variant _GLOBAL_DEF(const PropertyInfo &p_info, const Variant &p_default, bool p_restart_if_changed, bool p_ignore_value_in_docs, bool p_basic, bool p_internal) {
	Variant ret = _GLOBAL_DEF(p_info.name, p_default, p_restart_if_changed, p_ignore_value_in_docs, p_basic, p_internal);
	ProjectSettings::get_singleton()->set_custom_property_info(p_info);
	return ret;
}

/*
 * Return constant reference to internal autoloads map.
 *
 * @return - Reference to the map of autoload names and their associated info.
 */
const HashMap<StringName, ProjectSettings::AutoloadInfo> &ProjectSettings::get_autoload_list() const {
	return autoloads;
}

/*
 * Add autoload entry to autoload list.
 *
 * @param p_autoload - The AutoloadInfo object containing the name and setting.
 */
void ProjectSettings::add_autoload(const AutoloadInfo &p_autoload) {
	ERR_FAIL_COND_MSG(p_autoload.name == StringName(), "Trying to add autoload with no name.");
	autoloads[p_autoload.name] = p_autoload;
}

/*
 * Remove given autoload entry.
 *
 * @param p_autoload - The name of the autoload to remove.
 */
void ProjectSettings::remove_autoload(const StringName &p_autoload) {
	ERR_FAIL_COND_MSG(!autoloads.has(p_autoload), "Trying to remove non-existent autoload.");
	autoloads.erase(p_autoload);
}

/*
 * Check if given autoload entry exists.
 *
 * @param p_autoload - The name of the autolaod to check.
 *
 * @return - True, if autoload exists.
 *           False, otherwise.
 */
bool ProjectSettings::has_autoload(const StringName &p_autoload) const {
	return autoloads.has(p_autoload);
}

/*
 * Returns AutoloadInfo for given autoload name.
 *
 * @param p_name - The name of the autoload.
 *
 * @return - The AutoloadInfo structure that contains the autoload
 *           configuration.
 */
ProjectSettings::AutoloadInfo ProjectSettings::get_autoload(const StringName &p_name) const {
	ERR_FAIL_COND_V_MSG(!autoloads.has(p_name), AutoloadInfo(), "Trying to get non-existent autoload.");
	return autoloads[p_name];
}

/*
 * Returns reference to map of global groups.
 *
 * @return - Reference to a HashMap containing global group name mappings.
 */
const HashMap<StringName, String> &ProjectSettings::get_global_groups_list() const {
	return global_groups;
}

/*
 * Add global group with given name and description to list.
 *
 * @param p_name - Name of the global group.
 *
 * @param p_description - The description associated with the global group.
 */
void ProjectSettings::add_global_group(const StringName &p_name, const String &p_description) {
	ERR_FAIL_COND_MSG(p_name == StringName(), "Trying to add global group with no name.");
	global_groups[p_name] = p_description;
}

/*
 * Remove given global group.
 *
 * @param p_name - The name of the global group to remove.
 */
void ProjectSettings::remove_global_group(const StringName &p_name) {
	ERR_FAIL_COND_MSG(!global_groups.has(p_name), "Trying to remove non-existent global group.");
	global_groups.erase(p_name);
}

/*
 * Checks for given global group.
 *
 * @param p_name - Name of the global group to check.
 *
 * @return - True, if global group exists.
 *           False, otherwise.
 */
bool ProjectSettings::has_global_group(const StringName &p_name) const {
	return global_groups.has(p_name);
}

/*
 * Remove cached scene group for given path.
 *
 * @param p_path - The path whose scene group cache should be removed.
 */
void ProjectSettings::remove_scene_groups_cache(const StringName &p_path) {
	scene_groups_cache.erase(p_path);
}

/*
 * Caches scene group for given path/
 *
 * @param p_path - The path to associate with the cached scene group.
 *
 * @param p_cache - The set of group names to cache for the given path.
 */
void ProjectSettings::add_scene_groups_cache(const StringName &p_path, const HashSet<StringName> &p_cache) {
	scene_groups_cache[p_path] = p_cache;
}

/*
 * Save current scene cache to config file.
 */
void ProjectSettings::save_scene_groups_cache() {
	Ref<ConfigFile> config;
	config.instantiate();

	for (const auto &E : scene_groups_cache) {
		if (E.value.is_empty()) {
			continue;
		}

		Array list;
		const int size = E.value.size();

		// Pre-size array to avoid realloc during assignments.
		list.resize(size);

		int list_index = 0;

		for (const StringName &group : E.value) {
			list[list_index++] = group;
		}

		// This should not happen unless E.value.size() changes mid-loop.
		// Prevents you from saving a potentially malformed array.
		CRASH_COND(list_index != size);

		config->set_value(E.key, "groups", list);
	}

	const Error err = config->save(get_scene_groups_cache_path());

	if (err != OK) {
		ERR_PRINT(vformat("Failed to save scene groups cache."));
	}
}

/*
 * Return file path where scene groups cache should be.
 *
 * @return - Absolute path to the scene groups cache file.
 */
String ProjectSettings::get_scene_groups_cache_path() const {
	return get_project_data_path().path_join("scene_groups_cache.cfg");
}

/*
 * Loads cached scene group data, populates scene_group_cache map.
 */
void ProjectSettings::load_scene_groups_cache() {
	Ref<ConfigFile> config;
	config.instantiate();

	const Error err = config->load(get_scene_groups_cache_path());

	if (err != OK) {
		ERR_PRINT(vformat("Failed to load scene group cache."));
		return;
	}

	const Vector<String> scene_paths = config->get_sections();

	for (const String &E : scene_paths) {
		Variant raw_groups = config->get_value(E, "groups", Array());

		if (!raw_groups.is_array()) {
			ERR_PRINT(vformat("Scene group is not an array."));
			continue; // Skips invalid entry.
		}

		const Array scene_groups = raw_groups;

		HashSet<StringName> cache;

		for (const Variant &scene_group : scene_groups) {
			cache.insert(scene_group);
		}

		add_scene_groups_cache(E, cache);
	}
}

/*
 * Return cached mapping of scene paths.
 *
 * @return - A constant reference to the internal scene group's cache.
 */
const HashMap<StringName, HashSet<StringName>> &ProjectSettings::get_scene_groups_cache() const {
	return scene_groups_cache;
}

/*
 * ProjectSettings constructor that initializes the configuration.
 */
ProjectSettings::ProjectSettings() {
	// Initialization of engine variables should be done in the setup() method,
	// so that the values can be overridden from project.redot or project.binary.

	CRASH_COND_MSG(singleton != nullptr, "Instantiating a new ProjectSettings singleton is not supported.");
	singleton = this;

#ifdef TOOLS_ENABLED
	// Available only at runtime in editor builds. Needs to be processed before anything else to work properly.
	if (!Engine::get_singleton()->is_editor_hint()) {
		String editor_features = OS::get_singleton()->get_environment("GODOT_EDITOR_CUSTOM_FEATURES");
		if (!editor_features.is_empty()) {
			PackedStringArray feature_list = editor_features.split(",");
			for (const String &s : feature_list) {
				custom_features.insert(s.strip_edges());
			}
		}
	}
#endif

	GLOBAL_DEF_BASIC("application/config/name", "");
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::DICTIONARY, "application/config/name_localized", PROPERTY_HINT_LOCALIZABLE_STRING), Dictionary());
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "application/config/description", PROPERTY_HINT_MULTILINE_TEXT), "");
	GLOBAL_DEF_BASIC("application/config/version", "");
	GLOBAL_DEF_INTERNAL(PropertyInfo(Variant::STRING, "application/config/tags"), PackedStringArray());
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "application/run/main_scene", PROPERTY_HINT_FILE, "*.tscn,*.scn,*.res"), "");
	GLOBAL_DEF("application/run/disable_stdout", false);
	GLOBAL_DEF("application/run/disable_stderr", false);
	GLOBAL_DEF("application/run/print_header", true);
	GLOBAL_DEF("application/run/enable_alt_space_menu", false);
	GLOBAL_DEF_RST("application/config/use_hidden_project_data_directory", true);
	GLOBAL_DEF("application/config/use_custom_user_dir", false);
	GLOBAL_DEF("application/config/custom_user_dir_name", "");
	GLOBAL_DEF("application/config/project_settings_override", "");

	GLOBAL_DEF("application/run/main_loop_type", "SceneTree");
	GLOBAL_DEF("application/config/auto_accept_quit", true);
	GLOBAL_DEF("application/config/quit_on_go_back", true);

	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "accessibility/general/accessibility_support", PROPERTY_HINT_ENUM, "Auto (When Screen Reader is Running),Always Active,Disabled"), 0);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "accessibility/general/updates_per_second", PROPERTY_HINT_RANGE, "1,100,1"), 60);

	// The default window size is tuned to:
	// - Have a 16:9 aspect ratio,
	// - Have both dimensions divisible by 8 to better play along with video recording,
	// - Be displayable correctly in windowed mode on a 1366×768 display (tested on Windows 10 with default settings).
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "display/window/size/viewport_width", PROPERTY_HINT_RANGE, "1,7680,1,or_greater"), 1152); // 8K resolution
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "display/window/size/viewport_height", PROPERTY_HINT_RANGE, "1,4320,1,or_greater"), 648); // 8K resolution

	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "display/window/size/mode", PROPERTY_HINT_ENUM, "Windowed,Minimized,Maximized,Fullscreen,Exclusive Fullscreen"), 0);

	// Keep the enum values in sync with the `Window::WINDOW_INITIAL_POSITION_` enum.
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "display/window/size/initial_position_type", PROPERTY_HINT_ENUM, "Absolute:0,Center of Primary Screen:1,Center of Other Screen:3,Center of Screen With Mouse Pointer:4,Center of Screen With Keyboard Focus:5"), 1);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::VECTOR2I, "display/window/size/initial_position"), Vector2i());
	// Keep the enum values in sync with the `DisplayServer::SCREEN_` enum.
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "display/window/size/initial_screen", PROPERTY_HINT_RANGE, "0,64,1,or_greater"), 0);

	GLOBAL_DEF_BASIC("display/window/size/resizable", true);
	GLOBAL_DEF_BASIC("display/window/size/borderless", false);
	GLOBAL_DEF("display/window/size/always_on_top", false);
	GLOBAL_DEF("display/window/size/transparent", false);
	GLOBAL_DEF("display/window/size/extend_to_title", false);
	GLOBAL_DEF("display/window/size/no_focus", false);
	GLOBAL_DEF("display/window/size/sharp_corners", false);
	GLOBAL_DEF("display/window/size/minimize_disabled", false);
	GLOBAL_DEF("display/window/size/maximize_disabled", false);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "display/window/size/window_width_override", PROPERTY_HINT_RANGE, "0,7680,1,or_greater"), 0); // 8K resolution
	GLOBAL_DEF(PropertyInfo(Variant::INT, "display/window/size/window_height_override", PROPERTY_HINT_RANGE, "0,4320,1,or_greater"), 0); // 8K resolution

	GLOBAL_DEF("display/window/energy_saving/keep_screen_on", true);
	GLOBAL_DEF("animation/warnings/check_invalid_track_paths", true);
	GLOBAL_DEF("animation/warnings/check_angle_interpolation_type_conflicting", true);

	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "audio/buses/default_bus_layout", PROPERTY_HINT_FILE, "*.tres"), "res://default_bus_layout.tres");
	GLOBAL_DEF(PropertyInfo(Variant::INT, "audio/general/default_playback_type", PROPERTY_HINT_ENUM, "Stream,Sample"), 0);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "audio/general/default_playback_type.web", PROPERTY_HINT_ENUM, "Stream,Sample"), 1);
	GLOBAL_DEF_RST("audio/general/text_to_speech", false);
	GLOBAL_DEF_RST(PropertyInfo(Variant::FLOAT, "audio/general/2d_panning_strength", PROPERTY_HINT_RANGE, "0,2,0.01"), 0.5f);
	GLOBAL_DEF_RST(PropertyInfo(Variant::FLOAT, "audio/general/3d_panning_strength", PROPERTY_HINT_RANGE, "0,2,0.01"), 0.5f);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "audio/general/ios/session_category", PROPERTY_HINT_ENUM, "Ambient,Multi Route,Play and Record,Playback,Record,Solo Ambient"), 0);
	GLOBAL_DEF("audio/general/ios/mix_with_others", false);

	_add_builtin_input_map();

	// Keep the enum values in sync with the `DisplayServer::ScreenOrientation` enum.
	custom_prop_info["display/window/handheld/orientation"] = PropertyInfo(Variant::INT, "display/window/handheld/orientation", PROPERTY_HINT_ENUM, "Landscape,Portrait,Reverse Landscape,Reverse Portrait,Sensor Landscape,Sensor Portrait,Sensor");
	GLOBAL_DEF("display/window/subwindows/embed_subwindows", true);
	// Keep the enum values in sync with the `DisplayServer::VSyncMode` enum.
	custom_prop_info["display/window/vsync/vsync_mode"] = PropertyInfo(Variant::INT, "display/window/vsync/vsync_mode", PROPERTY_HINT_ENUM, "Disabled,Enabled,Adaptive,Mailbox");

	GLOBAL_DEF("display/window/frame_pacing/android/enable_frame_pacing", true);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "display/window/frame_pacing/android/swappy_mode", PROPERTY_HINT_ENUM, "pipeline_forced_on,auto_fps_pipeline_forced_on,auto_fps_auto_pipeline"), 2);

#ifdef DISABLE_DEPRECATED
	custom_prop_info["rendering/driver/threads/thread_model"] = PropertyInfo(Variant::INT, "rendering/driver/threads/thread_model", PROPERTY_HINT_ENUM, "Safe:1,Separate");
#else
	custom_prop_info["rendering/driver/threads/thread_model"] = PropertyInfo(Variant::INT, "rendering/driver/threads/thread_model", PROPERTY_HINT_ENUM, "Unsafe (deprecated),Safe,Separate");
#endif

#ifndef PHYSICS_2D_DISABLED
	GLOBAL_DEF("physics/2d/run_on_separate_thread", false);
#endif // PHYSICS_2D_DISABLED
#ifndef PHYSICS_3D_DISABLED
	GLOBAL_DEF("physics/3d/run_on_separate_thread", false);
#endif // PHYSICS_3D_DISABLED

	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "display/window/stretch/mode", PROPERTY_HINT_ENUM, "disabled,canvas_items,viewport"), "disabled");
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "display/window/stretch/aspect", PROPERTY_HINT_ENUM, "ignore,keep,keep_width,keep_height,expand"), "keep");
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::FLOAT, "display/window/stretch/scale", PROPERTY_HINT_RANGE, "0.5,8.0,0.01"), 1.0);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "display/window/stretch/scale_mode", PROPERTY_HINT_ENUM, "fractional,integer"), "fractional");

	GLOBAL_DEF(PropertyInfo(Variant::INT, "debug/settings/profiler/max_functions", PROPERTY_HINT_RANGE, "128,65535,1"), 16384);
	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "debug/settings/profiler/max_timestamp_query_elements", PROPERTY_HINT_RANGE, "256,65535,1"), 256);

	GLOBAL_DEF(PropertyInfo(Variant::BOOL, "compression/formats/zstd/long_distance_matching"), Compression::zstd_long_distance_matching);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "compression/formats/zstd/compression_level", PROPERTY_HINT_RANGE, "1,22,1"), Compression::zstd_level);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "compression/formats/zstd/window_log_size", PROPERTY_HINT_RANGE, "10,30,1"), Compression::zstd_window_log_size);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "compression/formats/zlib/compression_level", PROPERTY_HINT_RANGE, "-1,9,1"), Compression::zlib_level);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "compression/formats/gzip/compression_level", PROPERTY_HINT_RANGE, "-1,9,1"), Compression::gzip_level);

	GLOBAL_DEF("debug/settings/crash_handler/message",
			String("Please include this when reporting the bug to the project developer."));
	GLOBAL_DEF("debug/settings/crash_handler/message.editor",
			String("Please include this when reporting the bug on: https://github.com/Redot-Engine/redot-engine/issues"));
	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "rendering/occlusion_culling/bvh_build_quality", PROPERTY_HINT_ENUM, "Low,Medium,High"), 2);
	GLOBAL_DEF_RST("rendering/occlusion_culling/jitter_projection", true);

	GLOBAL_DEF_RST("internationalization/rendering/force_right_to_left_layout_direction", false);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "internationalization/rendering/root_node_layout_direction", PROPERTY_HINT_ENUM, "Based on Application Locale,Left-to-Right,Right-to-Left,Based on System Locale"), 0);
	GLOBAL_DEF_BASIC("internationalization/rendering/root_node_auto_translate", true);

	GLOBAL_DEF(PropertyInfo(Variant::INT, "gui/timers/incremental_search_max_interval_msec", PROPERTY_HINT_RANGE, "0,10000,1,or_greater"), 2000);
	GLOBAL_DEF(PropertyInfo(Variant::FLOAT, "gui/timers/tooltip_delay_sec", PROPERTY_HINT_RANGE, "0,5,0.01,or_greater"), 0.5);
#ifdef TOOLS_ENABLED
	GLOBAL_DEF("gui/timers/tooltip_delay_sec.editor_hint", 0.5);
#endif

	GLOBAL_DEF_BASIC("gui/common/snap_controls_to_pixels", true);
	GLOBAL_DEF_BASIC("gui/fonts/dynamic_fonts/use_oversampling", true);

	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "rendering/rendering_device/vsync/frame_queue_size", PROPERTY_HINT_RANGE, "2,3,1"), 2);
	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "rendering/rendering_device/vsync/swapchain_image_count", PROPERTY_HINT_RANGE, "2,4,1"), 3);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/rendering_device/staging_buffer/block_size_kb", PROPERTY_HINT_RANGE, "4,2048,1,or_greater"), 256);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/rendering_device/staging_buffer/max_size_mb", PROPERTY_HINT_RANGE, "1,1024,1,or_greater"), 128);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/rendering_device/staging_buffer/texture_upload_region_size_px", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 64);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/rendering_device/staging_buffer/texture_download_region_size_px", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 64);
	GLOBAL_DEF_RST(PropertyInfo(Variant::BOOL, "rendering/rendering_device/pipeline_cache/enable"), true);
	GLOBAL_DEF(PropertyInfo(Variant::FLOAT, "rendering/rendering_device/pipeline_cache/save_chunk_size_mb", PROPERTY_HINT_RANGE, "0.000001,64.0,0.001,or_greater"), 3.0);
	GLOBAL_DEF(PropertyInfo(Variant::INT, "rendering/rendering_device/vulkan/max_descriptors_per_pool", PROPERTY_HINT_RANGE, "1,256,1,or_greater"), 64);

	GLOBAL_DEF_RST("rendering/rendering_device/d3d12/max_resource_descriptors_per_frame", 16384);
	custom_prop_info["rendering/rendering_device/d3d12/max_resource_descriptors_per_frame"] = PropertyInfo(Variant::INT, "rendering/rendering_device/d3d12/max_resource_descriptors_per_frame", PROPERTY_HINT_RANGE, "512,262144");
	GLOBAL_DEF_RST("rendering/rendering_device/d3d12/max_sampler_descriptors_per_frame", 1024);
	custom_prop_info["rendering/rendering_device/d3d12/max_sampler_descriptors_per_frame"] = PropertyInfo(Variant::INT, "rendering/rendering_device/d3d12/max_sampler_descriptors_per_frame", PROPERTY_HINT_RANGE, "256,2048");
	GLOBAL_DEF_RST("rendering/rendering_device/d3d12/max_misc_descriptors_per_frame", 512);
	custom_prop_info["rendering/rendering_device/d3d12/max_misc_descriptors_per_frame"] = PropertyInfo(Variant::INT, "rendering/rendering_device/d3d12/max_misc_descriptors_per_frame", PROPERTY_HINT_RANGE, "32,4096");

	// The default value must match the minor part of the Agility SDK version
	// installed by the scripts provided in the repository
	// (check `misc/scripts/install_d3d12_sdk_windows.py`).
	// For example, if the script installs 1.613.3, the default value must be 613.
	GLOBAL_DEF_RST(PropertyInfo(Variant::INT, "rendering/rendering_device/d3d12/agility_sdk_version", PROPERTY_HINT_RANGE, "0,10000,1,or_greater,hide_slider"), 613);

	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "rendering/textures/canvas_textures/default_texture_filter", PROPERTY_HINT_ENUM, "Nearest,Linear,Linear Mipmap,Nearest Mipmap"), 1);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "rendering/textures/canvas_textures/default_texture_repeat", PROPERTY_HINT_ENUM, "Disable,Enable,Mirror"), 0);

	GLOBAL_DEF("collada/use_ambient", false);

	// Input settings
	GLOBAL_DEF_BASIC("input_devices/pointing/android/enable_long_press_as_right_click", false);
	GLOBAL_DEF_BASIC("input_devices/pointing/android/enable_pan_and_scale_gestures", false);
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::INT, "input_devices/pointing/android/rotary_input_scroll_axis", PROPERTY_HINT_ENUM, "Horizontal,Vertical"), 1);
	GLOBAL_DEF("input_devices/pointing/android/override_volume_buttons", false);
	GLOBAL_DEF_BASIC("input_devices/pointing/android/disable_scroll_deadzone", false);

	// These properties will not show up in the dialog. If you want to exclude whole groups, use add_hidden_prefix().
	GLOBAL_DEF_INTERNAL("application/config/features", PackedStringArray());
	GLOBAL_DEF_INTERNAL("internationalization/locale/translation_remaps", PackedStringArray());
	GLOBAL_DEF_INTERNAL("internationalization/locale/translations", PackedStringArray());
	GLOBAL_DEF_INTERNAL("internationalization/locale/translations_pot_files", PackedStringArray());
	GLOBAL_DEF_INTERNAL("internationalization/locale/translation_add_builtin_strings_to_pot", false);

#if !defined(NAVIGATION_2D_DISABLED) || !defined(NAVIGATION_3D_DISABLED)
	GLOBAL_DEF("navigation/world/map_use_async_iterations", true);

	GLOBAL_DEF("navigation/avoidance/thread_model/avoidance_use_multiple_threads", true);
	GLOBAL_DEF("navigation/avoidance/thread_model/avoidance_use_high_priority_threads", true);

	GLOBAL_DEF("navigation/pathfinding/max_threads", 4);

	GLOBAL_DEF("navigation/baking/use_crash_prevention_checks", true);
	GLOBAL_DEF("navigation/baking/thread_model/baking_use_multiple_threads", true);
	GLOBAL_DEF("navigation/baking/thread_model/baking_use_high_priority_threads", true);
#endif // !defined(NAVIGATION_2D_DISABLED) || !defined(NAVIGATION_3D_DISABLED)

	ProjectSettings::get_singleton()->add_hidden_prefix("input/");
}
