/**************************************************************************/
/*  project_settings_paths.cpp                                            */
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

#include "core/core_bind.h" // For Compression enum.
#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/file_access_pack.h"
#include "core/io/resource_uid.h"

/*
 * Returns project directory's name.
 *
 * @return - The directory name used for storing project-specific data.
 */
String ProjectSettings::get_project_data_dir_name() const {
	return project_data_dir_name;
}

/*
 * Returns full resource path for project.
 *
 * @return - The full resource path to the project data directory.
 */
String ProjectSettings::get_project_data_path() const {
	const String dir = get_project_data_dir_name();
	if (dir.is_empty()) {
		print_error("Project data directory name is empty. Using a fallback path.");
		return "res://invalid_project_data_directory";
	}
	return "res://" + dir;
}

/*
 * Returns base resource path where project is located.
 *
 * @return - The resource path for the current project.
 */
String ProjectSettings::get_resource_path() const {
	return resource_path;
}

/*
 * Returns full path to project's imported files directory.
 *
 * @return - The path to the imported subdirectory within the project data
 *           path.
 */
String ProjectSettings::get_imported_files_path() const {
	return get_project_data_path().path_join("imported");
}

/*
 * Converts a relative filesystem path into Redot path using "res://"
 *
 * @param p_path - The path to convert.
 *
 * @return - The localized path using "res://", if possible.
 *           Original path, if not possible.
 */
String ProjectSettings::localize_path(const String &p_path) const {
	// Normalize string by removing "." and ".."
	String path = p_path.simplify_path();

	if (resource_path.is_empty() || (path.is_absolute_path() && !path.begins_with(resource_path))) {
		return path;
	}

	// Check if we have a special path (like res://) or a protocol identifier.
	const int p = path.find("://");
	bool found = false;

	if (p > 0) {
		found = true;
		for (int i = 0; i < p; i++) {
			if (!is_ascii_alphanumeric_char(path[i])) {
				found = false;
				break;
			}
		}
	}

	if (found) {
		return path;
	}

	// Try to to resolve path relative to filesystem.
	Ref<DirAccess> dir = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);

	if (dir->change_dir(path) == OK) {
		// Get directory, normalize slashes.
		String cwd = dir->get_current_dir().replace_char('\\', '/');

		// Ensure trailing slash for both for comparison.
		const String res_path = resource_path.path_join("");
		cwd = cwd.path_join("");

		// Ensure cwd's location.
		if (!cwd.begins_with(res_path)) {
			return path;
		}

		// replace base with "res://" prefix.
		return cwd.replace_first(res_path, "res://");
	} else {
		// Fallback - Trying to localize parent directory.
		int sep = path.rfind_char('/');
		if (sep == -1) {
			return "res://" + path;
		}

		String parent = path.substr(0, sep);
		String plocal = localize_path(parent);

		if (plocal.is_empty()) {
			return "res://invalid"; // fallback indicator.
		}

		// Avoids duplicate slashes when appending child path.
		const String postfix = path.substr(plocal.ends_with("/") ? sep + 1 : sep);

		return plocal + postfix;
	}
}

/*
 * Converts virtual path to absolute path.
 *
 * @param p_path - The virtual path to convert.
 *
 * @return - The absolute filesystem path.
 */
String ProjectSettings::globalize_path(const String &p_path) const {
	if (p_path.begins_with("res://")) {
		// Replace "res://" with actual resource path, or strips it if no resource path.
		return resource_path.is_empty() ? p_path.replace("res://", "") : p_path.replace("res:/", resource_path);
	} else if (p_path.begins_with("uid://")) {
		// Replace "uid://" with actual resource path, or strips it if no resource path.
		const String path = ResourceUID::uid_to_path(p_path);
		return resource_path.is_empty() ? path.replace("res://", "") : path.replace("res:/", resource_path);
	} else if (p_path.begins_with("user://")) {
		// Replaces "user://" with user data directory.
		String data_dir = OS::get_singleton()->get_user_data_dir();
		return data_dir.is_empty() ? p_path.replace("user://", "") : p_path.replace("user:/", data_dir);
	}

	// Otherwise, it's not a virtual path.
	return p_path;
}

/*
 * Loads resource pack (.pck) into project.
 *
 * @param p_pack - Path to the .pack file to load.
 *
 * @param p_replace_files - Replace the existing files, if true.
 *
 * @param p_offset - Offset in the file to begin reading from.
 *
 * @return - True, if resource pack was loaded successfully.
 *           False, otherwise.
 */
bool ProjectSettings::load_resource_pack(const String &p_pack, bool p_replace_files, int p_offset) {
	return ProjectSettings::_load_resource_pack(p_pack, p_replace_files, p_offset, false);
}

/*
 * Actually loads the resource pack into the project.
 *
 * @param p_pack - Path to the .pack file.
 *
 * @param p_replace_files - Replace existing files with those in the pack, if
 *                          True.
 *
 * @param p_offset - Offset in the file to begin reading from.
 *
 * @param p_main_pack - Whether or not this is a main resource pack.
 *
 * @return - True, if resource pack successfully loaded.
 *           False, otherwise.
 */
bool ProjectSettings::_load_resource_pack(const String &p_pack, bool p_replace_files, int p_offset, bool p_main_pack) {
	if (PackedData::get_singleton()->is_disabled()) {
		return false;
	}

	if (p_pack == "res://") {
		// Loading the resource directory as a pack source is reserved for internal use only.
		return false;
	}

	if (!p_main_pack && !using_datapack && !OS::get_singleton()->get_resource_dir().is_empty()) {
		// Add the project's resource file system to PackedData so directory access keeps working when
		// the game is running without a main pack, like in the editor or on Android.
		PackedData::get_singleton()->add_pack_source(memnew(PackedSourceDirectory));
		PackedData::get_singleton()->add_pack("res://", false, 0);
		DirAccess::make_default<DirAccessPack>(DirAccess::ACCESS_RESOURCES);
		using_datapack = true;
	}

	const bool ok = PackedData::get_singleton()->add_pack(p_pack, p_replace_files, p_offset) == OK;

	if (!ok) {
		return false;
	}

	if (project_loaded) {
		// Refresh global script classes.
		refresh_global_class_list();

		// Rebuild UID cache
		ResourceUID::get_singleton()->load_from_cache(false);
	}

	// If the data pack was found, all directory access will be from here.
	if (!using_datapack) {
		DirAccess::make_default<DirAccessPack>(DirAccess::ACCESS_RESOURCES);
		using_datapack = true;
	}

	return true;
}

/*
 * Returns absolute path to global script class cache file.
 *
 * @return - Absolute path to the globa_script_class_cache.cfg file.
 */
String ProjectSettings::get_global_class_list_path() const {
	const String path = "global_script_class_cache.cfg";
	return get_project_data_path().path_join(path);
}

/*
 * Saves given array of global script classes.
 *
 * @param p_classes - Array of global script class dictionaries to store.
 */
void ProjectSettings::store_global_class_list(const Array &p_classes) {
	Ref<ConfigFile> config;
	config.instantiate();

	config->set_value("", "list", p_classes);

	const Error err = config->save(get_global_class_list_path());

	if (err != OK) {
		ERR_PRINT(vformat("Failed to save global class list."));
		return;
	}

	global_class_list = p_classes;
}
