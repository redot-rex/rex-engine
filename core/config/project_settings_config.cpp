/**************************************************************************/
/*  project_settings_config.cpp                                           */
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

#include "core/core_bind.h"
#include "core/input/input_map.h"
#include "core/io/config_file.h"
#include "core/io/dir_access.h"
#include "core/io/file_access_pack.h"
#include "core/io/marshalls.h"
#include "core/io/resource_uid.h"
#include "core/object/script_language.h"

#include "project_settings.h"

/*
 * Sets given project setting a given value, removes if value is null.
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_value - The value to assign to the setting. If null, setting will
 *                  be removed.
 *
 * @return - True, if setting was successfully updated or removed.
 *           False, otherwise.
 */
bool ProjectSettings::_set(const StringName &p_name, const Variant &p_value) {
	_THREAD_SAFE_METHOD_

	const String p_name_str = String(p_name);

	if (p_value.get_type() == Variant::NIL) {
		// Remove project setting entirely.
		props.erase(p_name);

		// Remove from autoloads, if autoload setting.
		if (p_name_str.begins_with("autoload/")) {
			const String node_name = p_name_str.split("/")[1];
			if (autoloads.has(node_name)) {
				remove_autoload(node_name);
			}
		} else if (p_name_str.begins_with("global_group/")) {
			const String group_name = p_name_str.get_slicec('/', 1);
			if (global_groups.has(group_name)) {
				remove_global_group(group_name);
			}
		}
	} else {
		// Handles custom features.
		if (p_name == CoreStringName(_custom_features)) {
			const Vector<String> custom_feature_array = String(p_value).split(",");
			for (int i = 0; i < custom_feature_array.size(); i++) {
				custom_features.insert(custom_feature_array[i]);
			}

			_version++;
			_queue_changed();
			return true;
		}

		{ // Feature overrides.
			const int dot = p_name_str.find_char('.');
			if (dot != -1) {
				const Vector<String> s = p_name_str.split(".");

				const int s_size = s.size();

				for (int i = 1; i < s_size; i++) {
					String feature = s[i].strip_edges();

					Pair<StringName, StringName> feature_override(feature, p_name);

					if (!feature_overrides.has(s[0])) {
						feature_overrides[s[0]] = LocalVector<Pair<StringName, StringName>>();
					}

					feature_overrides[s[0]].push_back(feature_override);
				}
			}
		}

		// Update or inserts the property.
		if (props.has(p_name)) {
			props[p_name].variant = p_value;
		} else {
			props[p_name] = VariantContainer(p_value, last_order++);
		}

		// Handles autoloads
		if (p_name_str.begins_with("autoload/")) {
			const String node_name = p_name_str.split("/")[1];
			AutoloadInfo autoload;
			autoload.name = node_name;
			const String path = p_value;
			if (path.begins_with("*")) {
				autoload.is_singleton = true;
				autoload.path = path.substr(1).simplify_path();
			} else {
				autoload.path = path.simplify_path();
			}
			add_autoload(autoload);
		} else if (p_name_str.begins_with("global_group/")) {
			const String group_name = p_name_str.get_slicec('/', 1);
			add_global_group(group_name, p_value);
		}
	}

	_version++;
	_queue_changed();
	return true;
}

/*
 * Obtain given project setting.
 *
 * @param p_name - The name of the project setting.
 *
 * @param r_ret - Reference to a Variant where the value will be stored.
 *
 * @return - True, if setting exists and was retrieved.
 *           False, otherwise.
 */
bool ProjectSettings::_get(const StringName &p_name, Variant &r_ret) const {
	_THREAD_SAFE_METHOD_

	if (!props.has(p_name)) {
		return false;
	}

	r_ret = props[p_name].variant;

	return true;
}

/*
 * Returns project setting value, while resolving feat. override
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_features - A list of custom feature names.
 *
 * @return - The setting value.
 *           If not found, an empty Variant.
 */
Variant ProjectSettings::get_setting_with_override_and_custom_features(const StringName &p_name, const Vector<String> &p_features) const {
	_THREAD_SAFE_METHOD_

	StringName name = p_name;

	HashSet<String> feature_lowered;

	// Cache lowered feature names for faster case-insensitive lookups.
	for (const String &f : p_features) {
		feature_lowered.insert(f.to_lower());
	}

	if (feature_overrides.has(name)) {
		const LocalVector<Pair<StringName, StringName>> &overrides = feature_overrides[name];

		for (const auto &pair : overrides) {
			const String feature_key = String(pair.first).to_lower();

			if (feature_lowered.has(feature_key) && props.has(pair.second)) {
				name = pair.second;
				break;
			}
		}
	}

	// cache property to avoid calling twice.
	auto prop_it = props.find(name);

	if (!prop_it) {
		WARN_PRINT("Property not found: " + String(name));
		return Variant();
	}

	return prop_it->value().variant;
}

/*
 * Return value of project setting, applying feat. overrides
 *
 * @param p_name - The name of the project setting.
 *
 * @return - If matching feature found, either the overridden
 *           setting value or the base setting value.
 *           If not found, returns an empty Variant.
 */
Variant ProjectSettings::get_setting_with_override(const StringName &p_name) const {
	_THREAD_SAFE_METHOD_

	const LocalVector<Pair<StringName, StringName>> *overrides = feature_overrides.getptr(p_name);

	if (overrides) {
		for (const auto &pair : *overrides) {
			if (!OS::get_singleton()->has_feature(pair.first)) {
				continue;
			}

			// Custom features are checked in OS.has_feature() already. No need to check twice.
			const auto *override_prop = props.find(pair.second);

			if (override_prop) {
				// Return base property value if no matching override found.
				return override_prop->get().variant;
			}
		}
	}

	const auto *base_prop = props.find(p_name);

	if (!base_prop) {
		WARN_PRINT(vformat("Property not found: '%s'.", p_name));
		return Variant();
	}

	return base_prop->get().variant;
}

/*
 * Convert older project files to latest format.
 *
 * @param p_from_version - The version number of the project file to convert
 * from.
 */
void ProjectSettings::_convert_to_last_version(int p_from_version) {
#ifndef DISABLE_DEPRECATED
	if (p_from_version <= 3) {
		// Converts the actions from array to dictionary (array of events to dictionary with deadzone + events)
		for (KeyValue<StringName, ProjectSettings::VariantContainer> &E : props) {
			const String key_str = String(E.key);
			Variant &value = E.value.variant;

			if (key_str.begins_with("input/") && value.get_type() == Variant::ARRAY) {
				Array array = value;
				Dictionary action;

				action["deadzone"] = Variant(0.5f);
				action["events"] = array;

				value = action;
			}
		}
	}
#endif // DISABLE_DEPRECATED
}

/*
 * Helper path trimmer for _setup().
 *
 * @param path - Reference to the path string to trim.
 */
void remove_trailing_slash(String &path) {
	if (!path.is_empty() && path.ends_with("/")) {
		path = path.substr(0, path.length() - 1);
	}
}

/*
 * This method is responsible for loading a project.godot file and/or data file
 * using the following merit order:
 *  - If using NetworkClient, try to lookup project file or fail.
 *  - If --main-pack was passed by the user (`p_main_pack`), load it or fail.
 *  - Search for project PCKs automatically. For each step we try loading a potential
 *    PCK, and if it doesn't work, we proceed to the next step. If any step succeeds,
 *    we try loading the project settings, and abort if it fails. Steps:
 *    o Bundled PCK in the executable.
 *    o [macOS only] PCK with same basename as the binary in the .app resource dir.
 *    o PCK with same basename as the binary in the binary's directory. We handle both
 *      changing the extension to '.pck' (e.g. 'win_game.exe' -> 'win_game.pck') and
 *      appending '.pck' to the binary name (e.g. 'linux_game' -> 'linux_game.pck').
 *    o PCK with the same basename as the binary in the current working directory.
 *      Same as above for the two possible PCK file names.
 *  - On relevant platforms (Android/iOS), lookup project file in OS resource path.
 *    If found, load it or fail.
 *  - Lookup project file in passed `p_path` (--path passed by the user), i.e. we
 *    are running from source code.
 *    If not found and `p_upwards` is true (--upwards passed by the user), look for
 *    project files in parent folders up to the system root (used to run a game
 *    from command line while in a subfolder).
 *    If a project file is found, load it or fail.
 *    If nothing was found, error out.
 *
 * @param p_path - The base path to search for the project file.
 *
 * @param p_main_pack - The path to a user-specified main PCK file, if any.
 *
 * @param p_upwards - If true, search parent directories for project file if
 *                    not found at p_path.
 *
 * @param p_ignore_override - If true, skip loading override configuration
 *                            file.
 *
 * @return OK, if project file was successfully loaded.
 *         The appropriate Error code otherwise.
 */
Error ProjectSettings::_setup(const String &p_path, const String &p_main_pack, bool p_upwards, bool p_ignore_override) {
	if (!OS::get_singleton()->get_resource_dir().is_empty()) {
		// OS will call ProjectSettings->get_resource_path which will be empty if not overridden!
		// If the OS would rather use a specific location, then it will not be empty.
		resource_path = OS::get_singleton()->get_resource_dir().replace_char('\\', '/');
		remove_trailing_slash(resource_path);
	}

	// Attempt with a user-defined main pack first

	if (!p_main_pack.is_empty()) {
		bool ok = _load_resource_pack(p_main_pack, false, 0, true);
		ERR_FAIL_COND_V_MSG(!ok, ERR_CANT_OPEN, vformat("Cannot open resource pack '%s'.", p_main_pack));

		Error err = _load_settings_text_or_binary("res://project.godot", "res://project.binary");
		if (err == OK && !p_ignore_override) {
			// Load override from location of the main pack
			// Optional, we don't mind if it fails
			_load_settings_text(p_main_pack.get_base_dir().path_join("override.cfg"));
		}
		return err;
	}

	const String exec_path = OS::get_singleton()->get_executable_path();

	if (!exec_path.is_empty()) {
		// We do several tests sequentially until one succeeds to find a PCK,
		// and if so, we attempt loading it at the end.

		// Attempt with PCK bundled into executable.
		bool found = _load_resource_pack(exec_path, false, 0, true);

		// Attempt with exec_name.pck.
		// (This is the usual case when distributing a Redot game.)
		String exec_dir = exec_path.get_base_dir();
		String exec_filename = exec_path.get_file();
		String exec_basename = exec_filename.get_basename();

		// Based on the OS, it can be the exec path + '.pck' (Linux w/o extension, macOS in .app bundle)
		// or the exec path's basename + '.pck' (Windows).
		// We need to test both possibilities as extensions for Linux binaries are optional
		// (so both 'mygame.bin' and 'mygame' should be able to find 'mygame.pck').

#ifdef MACOS_ENABLED
		if (!found) {
			// Attempt to load PCK from macOS .app bundle resources.
			found = _load_resource_pack(OS::get_singleton()->get_bundle_resource_dir().path_join(exec_basename + ".pck"), false, 0, true) || _load_resource_pack(OS::get_singleton()->get_bundle_resource_dir().path_join(exec_filename + ".pck"), false, 0, true);
		}
#endif

		if (!found) {
			// Try to load data pack at the location of the executable.
			// As mentioned above, we have two potential names to attempt.
			found = _load_resource_pack(exec_dir.path_join(exec_basename + ".pck"), false, 0, true) || _load_resource_pack(exec_dir.path_join(exec_filename + ".pck"), false, 0, true);
		}

		if (!found) {
			// If we couldn't find them next to the executable, we attempt
			// the current working directory. Same story, two tests.
			found = _load_resource_pack(exec_basename + ".pck", false, 0, true) || _load_resource_pack(exec_filename + ".pck", false, 0, true);
		}

		// If we opened our package, try and load our project.
		if (found) {
			Error err = _load_settings_text_or_binary("res://project.godot", "res://project.binary");
			if (err == OK && !p_ignore_override) {
				// Load overrides from the PCK and the executable location.
				// Optional, we don't mind if either fails.
				_load_settings_text("res://override.cfg");
				_load_settings_text(exec_path.get_base_dir().path_join("override.cfg"));
			}
			return err;
		}
	}

	// Try to use the filesystem for files, according to OS.
	// (Only Android -when reading from pck- and iOS use this.)
	// FIXME: Remove this iff we are removing mobile support.
	if (!OS::get_singleton()->get_resource_dir().is_empty()) {
		Error err = _load_settings_text_or_binary("res://project.godot", "res://project.binary");
		if (err == OK && !p_ignore_override) {
			// Optional, we don't mind if it fails.
			_load_settings_text("res://override.cfg");
		}
		return err;
	}

#ifdef MACOS_ENABLED
	// Attempt to load project file from macOS .app bundle resources.
	resource_path = OS::get_singleton()->get_bundle_resource_dir();
	if (!resource_path.is_empty()) {
		remove_trailing_slash(resource_path);
		Ref<DirAccess> d = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
		ERR_FAIL_COND_V_MSG(d.is_null(), ERR_CANT_CREATE, vformat("Cannot create DirAccess for path '%s'.", resource_path));
		d->change_dir(resource_path);

		Error err;

		err = _load_settings_text_or_binary(resource_path.path_join("project.godot"), resource_path.path_join("project.binary"));
		if (err == OK && !p_ignore_override) {
			// Optional, we don't mind if it fails.
			_load_settings_text(resource_path.path_join("override.cfg"));
			return err;
		}
	}
#endif

	// Nothing was found, try to find a project file in provided path (`p_path`)
	// or, if requested (`p_upwards`) in parent directories.

	Ref<DirAccess> d = DirAccess::create(DirAccess::ACCESS_FILESYSTEM);
	ERR_FAIL_COND_V_MSG(d.is_null(), ERR_CANT_CREATE, vformat("Cannot create DirAccess for path '%s'.", p_path));
	d->change_dir(p_path);

	String current_dir = d->get_current_dir();
	bool found = false;
	Error err;

	while (true) {
		// Set the resource path early so things can be resolved when loading.
		resource_path = current_dir.replace_char('\\', '/'); // Windows path to Unix path just in case.
		err = _load_settings_text_or_binary(current_dir.path_join("project.godot"), current_dir.path_join("project.binary"));
		if (err == OK && !p_ignore_override) {
			// Optional, we don't mind if it fails.
			_load_settings_text(current_dir.path_join("override.cfg"));
			found = true;
			break;
		}

		if (p_upwards) {
			// Try to load settings ascending through parent directories
			d->change_dir("..");
			if (d->get_current_dir() == current_dir) {
				break; // not doing anything useful
			}
			current_dir = d->get_current_dir();
		} else {
			break;
		}
	}

	if (!found) {
		return err;
	}

	remove_trailing_slash(resource_path);

	return OK;
}

/*
 * Public wrapper for _setup()
 *
 * @param p_path - The base path to search for the project file.
 *
 * @param p_main_pack - The path to a user-specified main PCK file.
 *
 * @param p_upwards - If true, search parent directories for project file.
 *
 * @param p_ignore_override - If true, skip loading override configuration
 *                            files.
 *
 * @return - OK, if setup was successful.
 *           An appropriate Error code otherwise.
 */
Error ProjectSettings::setup(const String &p_path, const String &p_main_pack, bool p_upwards, bool p_ignore_override) {
	Error err = _setup(p_path, p_main_pack, p_upwards, p_ignore_override);

	if (err == OK && !p_ignore_override) {
		String custom_settings = GLOBAL_GET("application/config/project_settings_override");

		if (!custom_settings.is_empty()) {
			_load_settings_text(custom_settings);
		}
	}

	// Updating the default value after the project settings have loaded.
	bool use_hidden_directory = GLOBAL_GET("application/config/use_hidden_project_data_directory");
	project_data_dir_name = (use_hidden_directory ? "." : "") + PROJECT_DATA_DIR_NAME_SUFFIX;

	// Cache compression settings early.
	{
		const String base = "compression/formats/";

		Compression::zstd_long_distance_matching = GLOBAL_GET(base + "zstd/long_distance_matching");
		Compression::zstd_level = GLOBAL_GET(base + "zstd/compression_level");
		Compression::zstd_window_log_size = GLOBAL_GET(base + "zstd/window_log_size");

		Compression::zlib_level = GLOBAL_GET(base + "zlib/compression_level");
		Compression::gzip_level = GLOBAL_GET(base + "gzip/compression_level");
	}

	load_scene_groups_cache();

	project_loaded = err == OK;

	return err;
}

/*
 * Loads project settings from a binary file format.
 *
 * @param p_path - The file path to the binary setting file.
 *
 * @return - OK, if successfully loaded.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_load_settings_binary(const String &p_path) {
	constexpr size_t HEADER_SIZE = 4;
	constexpr const char *EXPECTED_HEADER = "ECFG";

	// Sanity limits:
	constexpr uint32_t MAX_ENTRY_COUNT = 250000; // Safety cap for corrupted files.
	constexpr uint32_t MAX_STRING_SIZE = 1 << 20; // 1MB
	constexpr uint32_t MAX_BLOB_SIZE = 1 << 24; // 16MB

	Error err;

	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &err);

	if (err != OK) {
		return err;
	}

	uint8_t hdr[HEADER_SIZE];

	f->get_buffer(hdr, 4);

	if (memcmp(hdr, EXPECTED_HEADER, HEADER_SIZE) != 0) {
		// Verifies file header
		ERR_PRINT(vformat("Corrupt header in binary project.binary."));
		return ERR_FILE_CORRUPT;
	}

	uint32_t count = f->get_32();

	if (count > MAX_ENTRY_COUNT) {
		// Sanity check to guard against excessive entry counts.
		ERR_PRINT(vformat("Too many entries."));
	}

	for (uint32_t i = 0; i < count; i++) {
		uint32_t slen = f->get_32();
		// Sanity check to guard against extreme string lengths
		if (slen == 0 || slen > MAX_STRING_SIZE) {
			ERR_PRINT(vformat("Invalid key string length in binary settings."));
			continue;
		}

		CharString cs;
		cs.resize(slen + 1);
		cs[slen] = 0;
		f->get_buffer((uint8_t *)cs.ptr(), slen);

		String key = String::utf8(cs.ptr(), slen);

		uint32_t vlen = f->get_32();
		// Sanity  check to guard against invalid/excessive value blobs
		if (vlen == 0 || vlen > MAX_BLOB_SIZE) {
			ERR_PRINT(vformat("Invalid value blob size for key: '%s'", key));
			continue;
		}

		Vector<uint8_t> d;
		d.resize(vlen);
		f->get_buffer(d.ptrw(), vlen);

		Variant value;
		err = decode_variant(value, d.ptr(), d.size(), nullptr, true);
		// Continue anyways, but print error for debugging.
		if (err != OK) {
			ERR_PRINT(vformat("Failed to decode value for key: '%s'", key));
			continue;
		}

		set(key, value);
	}

	return OK;
}

/*
 * Load project settings from a text file format.
 *
 * @param p_path - The file path to the text-based project settings file.
 *
 * @return - OK, if setting was successfully loaded.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_load_settings_text(const String &p_path) {
	Error err;
	Ref<FileAccess> f = FileAccess::open(p_path, FileAccess::READ, &err);

	if (f.is_null()) {
		// FIXME: Above 'err' error code is ERR_FILE_CANT_OPEN if the file is missing
		// This needs to be streamlined if we want decent error reporting
		return ERR_FILE_NOT_FOUND;
	}

	VariantParser::StreamFile stream;
	stream.f = f;

	String assign;
	Variant value;
	VariantParser::Tag next_tag;

	int lines = 0;
	String error_text;
	String section;
	int config_version = 0;

	constexpr const char *KEY_CONFIG_VERSION = "config_version";

	while (true) {
		assign = Variant();
		next_tag.fields.clear();
		next_tag.name = String();

		err = VariantParser::parse_tag_assign_eof(&stream, lines, error_text, next_tag, assign, value, nullptr, true);

		if (err == ERR_FILE_EOF) {
			// If we're loading a project.godot from source code, we can operate some
			// ProjectSettings conversions if need be.
			_convert_to_last_version(config_version);
			last_save_time = FileAccess::get_modified_time(get_resource_path().path_join("project.godot"));
			return OK;
		}

		ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Error parsing '%s' at line %d: %s File might be corrupted.", p_path, lines, error_text));

		if (!assign.is_empty()) {
			if (section.is_empty() && assign == KEY_CONFIG_VERSION) {
				config_version = value;
				ERR_FAIL_COND_V_MSG(config_version > CONFIG_VERSION,
						ERR_FILE_CANT_OPEN,
						vformat("Can't open project at '%s', its `config_version` (%d) is from a more recent and incompatible version of the engine. Expected config version: %d.",
								p_path, config_version, CONFIG_VERSION));

			} else {
				const String full_key = section.is_empty() ? assign : section + "/" + assign;
				set(full_key, value);
			}
		} else if (!next_tag.name.is_empty()) {
			section = next_tag.name;
		}
	}
}

/*
 * Tries to load binary first, falls back to text.
 *
 * @param p_text_path - The file path to the text-based project settings file.
 *
 * @param p_bin_path - The file path to the binary project settings file.
 *
 * @return - OK, if either format was loaded successfully.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_load_settings_text_or_binary(const String &p_text_path, const String &p_bin_path) {
	// Attempt first to load the binary project.godot file.
	Error err = _load_settings_binary(p_bin_path);

	if (err == OK) {
		return OK;
	} else if (err != ERR_FILE_NOT_FOUND) {
		// If the file exists but can't be loaded, we want to know it.
		ERR_PRINT(vformat("Couldn't load file '%s', error code %d.", p_bin_path, err));
	}

	// Fallback to text-based project.godot file if binary was not found.
	err = _load_settings_text(p_text_path);

	if (err == OK) {
		return OK;
	} else if (err != ERR_FILE_NOT_FOUND) {
		ERR_PRINT(vformat("Couldn't load file '%s', error code %d.", p_text_path, err));
	}

	return err;
}

/*
 * Load a custom project setting file from given path.
 *
 * @param p_path - The path to the custom project settings file.
 *
 * @return - OK, if the file was successfully loaded.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::load_custom(const String &p_path) {
	if (p_path.ends_with(".binary")) {
		return _load_settings_binary(p_path);
	}

	return _load_settings_text(p_path);
}

/*
 * Save project settings in binary form to given path.
 *
 * @param p_file - Path to the output binary file.
 *
 * @param p_props - Grouped list of project setting names.
 *
 * @param p_custom - Map of custom setting values to override defaults.
 *
 * @param p_custom_features - Optional, comma-separated list of custom
 *                            features.
 *
 * @return - OK, if successful.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_save_settings_binary(const String &p_file, const RBMap<String, List<String>> &p_props, const CustomMap &p_custom, const String &p_custom_features) {
	Error err;
	Ref<FileAccess> file = FileAccess::open(p_file, FileAccess::WRITE, &err);
	ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Couldn't save project.binary at '%s'.", p_file));

	constexpr int MAX_PROJECT_PROPERTIES = 1 << 24; // Unrealistic INT_MAX.
	uint8_t hdr[4] = { 'E', 'C', 'F', 'G' };
	file->store_buffer(hdr, 4);

	int count = 0;

	for (const auto &E : p_props) {
		count += E.value.size();
	}

	if (!p_custom_features.is_empty()) {
		// Sanity check for overflow/unrealistic values
		ERR_FAIL_COND_V(count < 0 || count > MAX_PROJECT_PROPERTIES, ERR_INVALID_DATA);
		// Store how many properties are saved, add one for custom features, which must always go first.
		file->store_32(uint32_t(count + 1));

		String key = CoreStringName(_custom_features);
		file->store_pascal_string(key);

		int len;
		err = encode_variant(p_custom_features, nullptr, len, false);
		ERR_FAIL_COND_V(err != OK, err);

		Vector<uint8_t> buff;
		buff.resize(len);

		err = encode_variant(p_custom_features, buff.ptrw(), len, false);
		ERR_FAIL_COND_V(err != OK, err);

		file->store_32(uint32_t(len));
		file->store_buffer(buff.ptr(), buff.size());

	} else {
		// Store how many properties are saved.
		file->store_32(uint32_t(count));
	}

	for (const auto &E : p_props) {
		for (const String &key : E.value) {
			String k = E.key.is_empty() ? key : E.key + "/" + key;

			Variant value;

			if (p_custom.has(k)) {
				value = p_custom[k];
			} else {
				value = get(k);
			}

			file->store_pascal_string(k);

			int len;
			err = encode_variant(value, nullptr, len, true);
			ERR_FAIL_COND_V_MSG(err != OK, ERR_INVALID_DATA, "Error when trying to encode Variant.");

			Vector<uint8_t> buff;
			buff.resize(len);

			err = encode_variant(value, buff.ptrw(), len, true);
			ERR_FAIL_COND_V_MSG(err != OK, ERR_INVALID_DATA, "Error when trying to encode Variant.");

			file->store_32(uint32_t(len));
			file->store_buffer(buff.ptr(), buff.size());
		}
	}

	return OK;
}

/*
 * Save  project settings in text format.
 *
 * @param p_file - The path to the output text file.
 *
 * @param p_props - Grouped list of project setting names.
 *
 * @param p_custom - Map of custom setting values to override defaults.
 *
 * @param p_custom_features - Optional, comma-separated list of custom
 *                            features.
 *
 * @return - OK, if successful.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_save_settings_text(const String &p_file, const RBMap<String, List<String>> &p_props, const CustomMap &p_custom, const String &p_custom_features) {
	Error err;
	Ref<FileAccess> file = FileAccess::open(p_file, FileAccess::WRITE, &err);

	ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Couldn't save project.godot - %s.", p_file));

	file->store_line("; Engine configuration file.");
	file->store_line("; It's best edited using the editor UI and not directly,");
	file->store_line("; since the parameters that go here are not all obvious.");
	file->store_line(";");
	file->store_line("; Format:");
	file->store_line(";   [section] ; section goes between []");
	file->store_line(";   param=value ; assign values to parameters");
	file->store_line("");

	file->store_string("config_version=" + itos(CONFIG_VERSION) + "\n");

	if (!p_custom_features.is_empty()) {
		file->store_string("custom_features=\"" + p_custom_features + "\"\n");
	}

	file->store_string("\n");

	for (const auto &E : p_props) {
		if (E.key != p_props.begin()->key) {
			file->store_string("\n");
		}

		if (!E.key.is_empty()) {
			file->store_string("[" + E.key + "]\n\n");
		}

		for (const String &F : E.value) {
			String key = F;

			if (!E.key.is_empty()) {
				key = E.key + "/" + key;
			}

			Variant value;

			if (p_custom.has(key)) {
				value = p_custom[key];
			} else {
				value = get(key);
			}

			String vstr;

			VariantWriter::write_to_string(value, vstr);
			file->store_string(F.property_name_encode() + "=" + vstr + "\n");
		}
	}

	return OK;
}

/*
 * Saves project settings using custom `.bnd` format.
 *
 * @param p_file - Path to the output .bnd file.
 *
 * @return - OK, if successful.
 *           Appropriate Error code, otherwise.
 */
Error ProjectSettings::_save_custom_bnd(const String &p_file) { // add other params as dictionary and array?
	return save_custom(p_file);
}

/*
 * Define global setting with with metadata flags; sets defaults as needed.
 *
 * @param p_var - Name of the setting.
 *
 * @param p_default - Default value to use, if setting doesn't exist.
 *
 * @param p_restart_if_changed - If true, a restart is required if value is
 *                               changed.
 *
 * @param p_ignore_value_in_docx -  If true, value should be ignored in
 *                                  generated documentation.
 *
 * @param p_basic - Marks setting as basic.
 *
 * @param p_internal - Marks setting as internal.
 *
 * @return - The current value of the setting after initialization.
 */
Variant _GLOBAL_DEF(const String &p_var, const Variant &p_default, bool p_restart_if_changed, bool p_ignore_value_in_docs, bool p_basic, bool p_internal) {
	Variant ret;

	// Avoids repeated singleton calls
	ProjectSettings *ps = ProjectSettings::get_singleton();
	// Guards against null.
	ERR_FAIL_NULL_V(ps, Variant());

	// Set default if setting doesn't exist
	if (!ps->has_setting(p_var)) {
		ps->set(p_var, p_default);
	}

	ret = GLOBAL_GET(p_var);

	ps->set_initial_value(p_var, p_default);
	ps->set_builtin_order(p_var);
	ps->set_as_basic(p_var, p_basic);
	ps->set_restart_if_changed(p_var, p_restart_if_changed);
	ps->set_ignore_value_in_docs(p_var, p_ignore_value_in_docs);
	ps->set_as_internal(p_var, p_internal);

	return ret;
}

/*
 * Returns reference to custom property info map.
 *
 * @return - Reference to the custom property info map.
 */
const HashMap<StringName, PropertyInfo> &ProjectSettings::get_custom_property_info() const {
	return custom_prop_info;
}

/*
 * Check if engine is using data pack for resources.
 *
 * @return - True, if using a datapack.
 *           False, otherwise.
 */
bool ProjectSettings::is_using_datapack() const {
	return using_datapack;
}

/*
 * Check if project was successfully loaded.
 *
 * @return - True, if project was loaded.
 *           False, otherwise.
 */
bool ProjectSettings::is_project_loaded() const {
	return project_loaded;
}

/*
 * Checks if property can be reverted to init value.
 *
 * @param p_name - Name of the property.
 *
 * @return - True, if property exists and can be reverted.
 *           False, otherwise.
 */
bool ProjectSettings::_property_can_revert(const StringName &p_name) const {
	return props.has(p_name);
}

/*
 * Returns init value of a property, if it exists.
 *
 * @param p_name - Name of the property.
 *
 * @param r_property - Output parameter to store the initial value.
 *
 * @return - True, if property exists and its initial value was retrieved.
 *           False, otherwise.
 */
bool ProjectSettings::_property_get_revert(const StringName &p_name, Variant &r_property) const {
	const RBMap<StringName, ProjectSettings::VariantContainer>::Element *value = props.find(p_name);

	if (value) {
		// Ensures COW Variant.
		r_property = value->value().initial.duplicate();
		return true;
	}

	return false;
}

/*
 * Set given project setting to given value.
 *
 * @param p_setting - Name of the setting to modify.
 *
 * @param p_value - Value to assign to the setting.
 */
void ProjectSettings::set_setting(const String &p_setting, const Variant &p_value) {
	set(p_setting, p_value);
}

/*
 * Reload, applies all global script classes from project settings.
 */
void ProjectSettings::refresh_global_class_list() {
	// This is called after mounting a new PCK file to pick up class changes.
	is_global_class_list_loaded = false; // Make sure we read from the freshly mounted PCK.

	Array script_classes = get_global_class_list();

	constexpr const char *required_keys[] = {
		"class", "base", "language", "path", "is_abstract", "is_tool"
	};

	for (int i = 0; i < script_classes.size(); i++) {
		Dictionary c = script_classes[i];

		bool valid = true;

		// I'd rather us iterate to make sure the key is valid.
		for (const char *key : required_keys) {
			if (!c.has(key)) {
				valid = false;
				break;
			}
		}

		if (!valid) {
			continue;
		}

		ScriptServer::add_global_class(c["class"], c["base"], c["language"], c["path"], c["is_abstract"], c["is_tool"]);
	}
}

/*
 * Loads, returns list of global script classes for project.
 *
 * @return - Typed array of dictionaries representing global script classes.
 */
TypedArray<Dictionary> ProjectSettings::get_global_class_list() {
	if (is_global_class_list_loaded) {
		// Return cached value if already loaded
		return global_class_list;
	}

	Ref<ConfigFile> config;
	config.instantiate();

	Error err = config->load(get_global_class_list_path());

	if (err == OK) {
		global_class_list = config->get_value("", "list", Array());
	} else {
#ifndef TOOLS_ENABLED
		// Script classes can't be recreated in exported project, so print an error.
		ERR_PRINT("Could not load global script cache.");
#endif
	}

	// File read succeeded or failed. If it failed, assume everything is still okay.
	// We will later receive updated class data in store_global_class_list().
	is_global_class_list_loaded = true;

	return global_class_list;
}

/*
 * Add engine-defined input actions to project settings automatically.
 */
void ProjectSettings::_add_builtin_input_map() {
	// Cache to only call get_singleton once.
	InputMap *im = InputMap::get_singleton();

	// Verify InputMap initialized.
	if (!im) {
		return;
	}

	HashMap<String, List<Ref<InputEvent>>> builtins = im->get_builtins();

	const int builtin_size = builtins.size();
	input_presets.resize(builtin_size);
	int presets_helper = 0;

	for (auto &E : builtins) {
		const List<Ref<InputEvent>> &src = E.value;
		const int src_size = src.size();

		Array events;
		events.resize(src_size);

		int events_helper = 0;

		// Convert to Array.
		for (const Ref<InputEvent> &event : E.value) {
			// We know the amount, so why use push_back()?
			events[events_helper++] = event;
		}

		Dictionary action;
		action["deadzone"] = Variant(InputMap::DEFAULT_TOGGLE_DEADZONE);
		action["events"] = events;

		const String action_name = "input/" + String(E.key);
		GLOBAL_DEF(action_name, action);

		input_presets.write[presets_helper++] = action_name;
	}
}
