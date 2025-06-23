/**************************************************************************/
/*  project_settings_features.cpp                                         */
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

#include "core/io/dir_access.h"
#include "core/object/script_language.h"
#include "core/templates/rb_set.h"
#include "core/version.h"

#ifdef TOOLS_ENABLED
#include "modules/modules_enabled.gen.h" // For mono.
#endif // TOOLS_ENABLED

#ifdef TOOLS_ENABLED
/*
 * Returns the features that a project must have when opened with this build of Redot.
 * This is used by the project manager to provide the initial_settings for config/features.
 *
 * @return - PackedStringArray List of required features.
 */
const PackedStringArray ProjectSettings::get_required_features() {
	PackedStringArray features;
	features.append(REDOT_VERSION_BRANCH);
#ifdef REAL_T_IS_DOUBLE
	features.append("Double Precision");
#endif
	return features;
}

/*
 * Returns the features supported by this build of Redot. Includes all required features.
 *
 * @return - PackedStringArray List of supported features.
 */
const PackedStringArray ProjectSettings::_get_supported_features() {
	PackedStringArray features = get_required_features();
#ifdef MODULE_MONO_ENABLED
	features.append("C#");
#endif
	// Allow pinning to a specific patch number or build type by marking
	// them as supported. They're only used if the user adds them manually.
	features.append(REDOT_VERSION_BRANCH "." _MKSTR(REDOT_VERSION_PATCH));
	features.append(REDOT_VERSION_FULL_CONFIG);
	features.append(REDOT_VERSION_FULL_BUILD);

#ifdef RD_ENABLED
	// Render-specific features
	features.append("Forward Plus");
	features.append("Mobile");
#endif

#ifdef GLES3_ENABLED
	features.append("GL Compatibility");
#endif
	return features;
}

/*
 * Returns the features that this project needs but this build of Redot lacks.
 *
 * @param p_project_features - List of features required by the project.
 *
 * @return - PackedStringArray List of unsupported features.
 */
const PackedStringArray ProjectSettings::get_unsupported_features(const PackedStringArray &p_project_features) {
	PackedStringArray unsupported_features;
	PackedStringArray supported_features = singleton->_get_supported_features();

	for (const String &feature : p_project_features) {
		// Skips legacy Vulkan flags that aren't enforced anymore
		if (feature.begins_with("Vulkan")) {
			continue;
		}

		// Check whether or not current feature is supported.
		if (!supported_features.has(feature)) {
			unsupported_features.append(feature);
		}
	}

	unsupported_features.sort();
	return unsupported_features;
}

/*
 * Returns the features that both this project has and this build of Redot has, ensuring required features exist.
 *
 * @param p_project_features - List of features required by the project.
 *
 * @return - PackedStringArray, filtered and sorted list of supported features.
 */
const PackedStringArray ProjectSettings::_trim_to_supported_features(const PackedStringArray &p_project_features) {
	PackedStringArray features = PackedStringArray(p_project_features);
	PackedStringArray supported_features = _get_supported_features();

	// Remove unsupported features.
	for (int i = p_project_features.size() - 1; i > -1; i--) {
		if (!supported_features.has(p_project_features[i])) {
			features.remove_at(i);
		}
	}

	PackedStringArray required_features = get_required_features();

	// Add required features if not present.
	for (const auto &req_feature : required_features) {
		if (!features.has(req_feature)) {
			features.append(req_feature);
		}
	}

	features.sort();
	return features;
}

/*
 * Checks given directory for .csproj
 *
 * @param p_root_dir - The directory path to check.
 *
 * @return - True, if .csproj file is found.
 *           False, otherwise.
 */
bool _csproj_exists(const String &p_root_dir) {
	Ref<DirAccess> dir = DirAccess::open(p_root_dir);
	ERR_FAIL_COND_V(dir.is_null(), false);

	dir->list_dir_begin();

	while (true) {
		String file_name = dir->_get_next(); // `_get_next()` skips dot and hidden files.

		if (file_name.is_empty()) {
			break;
		}

		if (!dir->current_is_dir() && file_name.get_extension() == "csproj") {
			dir->list_dir_end(); // clears out list.
			return true;
		}
	}

	dir->list_dir_end();
	return false;
}
#endif // TOOLS_ENABLED

/*
 * Saves project setting to custom file path.
 *
 * @param p_path - The file path to save the settings to.
 *
 * @param p_custom - A map of custom settings to save.
 *
 * @param p_custom_features - A list of custom project features to include.
 *
 * @param p_merge_with_current - Merge current settings with custom ones, if True.
 *
 * @return - OK, on success.
 *           Appropriate Error code, on failure.
 */
Error ProjectSettings::save_custom(const String &p_path, const CustomMap &p_custom, const Vector<String> &p_custom_features, bool p_merge_with_current) {
	ERR_FAIL_COND_V_MSG(p_path.is_empty(), ERR_INVALID_PARAMETER, "Project settings save path cannot be empty.");

#ifdef TOOLS_ENABLED
	PackedStringArray project_features = get_setting("application/config/features");
	// If there is no feature list currently present, force one to generate.
	if (project_features.is_empty()) {
		project_features = ProjectSettings::get_required_features();
	}
	// Check the rendering API.
	const String rendering_api = has_setting("rendering/renderer/rendering_method") ? (String)get_setting("rendering/renderer/rendering_method") : String();
	if (!rendering_api.is_empty()) {
		// Add the rendering API as a project feature if it doesn't already exist.
		if (!project_features.has(rendering_api)) {
			project_features.append(rendering_api);
		}
	}
	// Check for the existence of a csproj file.
	if (_csproj_exists(get_resource_path())) {
		// If there is a csproj file, add the C# feature if it doesn't already exist.
		if (!project_features.has("C#")) {
			project_features.append("C#");
		}
	} else {
		// If there isn't a csproj file, remove the C# feature if it exists.
		if (project_features.has("C#")) {
			project_features.remove_at(project_features.find("C#"));
		}
	}
	project_features = _trim_to_supported_features(project_features);
	set_setting("application/config/features", project_features);
#endif // TOOLS_ENABLED

	RBSet<_VCSort> vclist;

	if (p_merge_with_current) {
		for (const auto &G : props) {
			const VariantContainer *v = &G.value;

			if (v->hide_from_editor || p_custom.has(G.key) || v->variant == v->initial) {
				continue;
			}

			_VCSort vc;
			vc.name = G.key; //*k;
			vc.order = v->order;
			vc.type = v->variant.get_type();
			vc.flags = PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE;

			vclist.insert(vc);
		}
	}

	for (const auto &E : p_custom) {
		// Lookup global prop to store in the same order
		RBMap<StringName, VariantContainer>::Iterator global_prop = props.find(E.key);

		_VCSort vc;
		vc.name = E.key;
		vc.order = global_prop ? global_prop->value.order : 0xFFFFFFF;
		vc.type = E.value.get_type();
		vc.flags = PROPERTY_USAGE_STORAGE;
		vclist.insert(vc);
	}

	RBMap<String, List<String>> save_props;

	for (const auto &E : vclist) {
		String category = E.name;
		String name = E.name;

		int div = category.find_char('/');

		if (div < 0) {
			category = "";
		} else {
			category = category.substr(0, div);
			name = name.substr(div + 1);
		}
		save_props[category].push_back(name);
	}

	String save_features;

	for (const auto &f : p_custom_features) {
		save_features += f.strip_edges().remove_char('\"') + ",";
	}

	if (!save_features.is_empty()) {
		// Remove trailing comma.
		save_features = save_features.substr(0, save_features.length() - 1);
	}

	if (p_path.ends_with(".godot") || p_path.ends_with("override.cfg")) {
		return _save_settings_text(p_path, save_props, p_custom, save_features);
	} else if (p_path.ends_with(".binary")) {
		return _save_settings_binary(p_path, save_props, p_custom, save_features);
	} else {
		ERR_FAIL_V_MSG(ERR_FILE_UNRECOGNIZED, vformat("Unknown config file format: '%s'.", p_path));
	}
}

/*
 * Check if given custom feature exists in project.
 *
 * @param p_feature - The name of the custom feature to check.
 *
 * @return - True, if custom feature exists.
 *           False, otherwise.
 */
bool ProjectSettings::has_custom_feature(const String &p_feature) const {
	return custom_features.has(p_feature);
}

#ifdef TOOLS_ENABLED
void ProjectSettings::get_argument_options(const StringName &p_function, int p_idx, List<String> *r_options) const {
	const String pf = p_function;
	if (p_idx == 0) {
		if (pf == "has_setting" || pf == "set_setting" || pf == "get_setting" || pf == "get_setting_with_override" ||
				pf == "set_order" || pf == "get_order" || pf == "set_initial_value" || pf == "set_as_basic" ||
				pf == "set_as_internal" || pf == "set_restart_if_changed" || pf == "clear") {
			for (const KeyValue<StringName, VariantContainer> &E : props) {
				if (E.value.hide_from_editor) {
					continue;
				}

				r_options->push_back(String(E.key).quote());
			}
		}
	}
	Object::get_argument_options(p_function, p_idx, r_options);
}
#endif
