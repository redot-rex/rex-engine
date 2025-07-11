/**************************************************************************/
/*  project_settings_access.cpp                                           */
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

#include "core/io/file_access_pack.h"

#include "project_settings.h"

/*
 * Set init value of a project setting.
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_value - The initial value to assign.
 */
void ProjectSettings::set_initial_value(const String &p_name, const Variant &p_value) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	// Duplicate so that if value is array or dictionary, changing the setting will not change the stored initial value.
	props[p_name].initial = p_value.duplicate();
}

/*
 * Set if restart is needed if value is changed.
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_restart - True, if restart is required when the setting changes.
 */
void ProjectSettings::set_restart_if_changed(const String &p_name, bool p_restart) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	props[p_name].restart_if_changed = p_restart;
}

/*
 * Marks if given project is basic or not.
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_basic - True, marks setting as basic.
 */
void ProjectSettings::set_as_basic(const String &p_name, bool p_basic) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	props[p_name].basic = p_basic;
}

/*
 * Marks project setting as internal.
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_internal - True marks setting as internal.
 */
void ProjectSettings::set_as_internal(const String &p_name, bool p_internal) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	props[p_name].internal = p_internal;
}

/*
 * Sets whether values should be ignored in generated docs. (debug only)
 *
 * @param p_name - The name of the project setting.
 *
 * @param p_ignore - True marks value to be ignored in documentation.
 */
void ProjectSettings::set_ignore_value_in_docs(const String &p_name, bool p_ignore) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
#ifdef DEBUG_METHODS_ENABLED
	props[p_name].ignore_value_in_docs = p_ignore;
#endif
}

/*
 * Returns whether or not setting's values should be ignored in generated docs.
 *
 * @param p_name - The name of the project setting.
 *
 * @return - True, if value is ignored in documentation.
 *           False, if otherwise.
 */
bool ProjectSettings::get_ignore_value_in_docs(const String &p_name) const {
	ERR_FAIL_COND_V_MSG(!props.has(p_name), false, vformat("Request for nonexistent project setting: '%s'.", p_name));
#ifdef DEBUG_METHODS_ENABLED
	return props[p_name].ignore_value_in_docs;
#else
	return false;
#endif
}

/*
 * Add prefix to list of setting name prefixes that ought to be hidden.
 *
 * @param p_prefix - The prefix to mark as hidden.
 */
void ProjectSettings::add_hidden_prefix(const String &p_prefix) {
	ERR_FAIL_COND_MSG(hidden_prefixes.has(p_prefix), vformat("Hidden prefix '%s' already exists.", p_prefix));
	hidden_prefixes.push_back(p_prefix);
}

/*
 * Checks if project setting contains a variable with given variable.
 *
 * @param p_var - The name of the setting to check.
 *
 * @return - True, if setting exists.
 *           False, otherwise.
 */
bool ProjectSettings::has_setting(const String &p_var) const {
	_THREAD_SAFE_METHOD_

	return props.has(p_var);
}

/*
 * Returns display order of given project setting.
 *
 * @param p_name - The name of the project setting.
 *
 * @return - The ordering index of the setting.
 *           -1, if setting does not exist.
 */
int ProjectSettings::get_order(const String &p_name) const {
	ERR_FAIL_COND_V_MSG(!props.has(p_name), -1, vformat("Request for nonexistent project setting: '%s'.", p_name));
	return props[p_name].order;
}

/*
 * Set ordering index for given project setting.
 *
 * @param p_name - The name of the setting.
 *
 * @param p_order - The order index to assign.
 */
void ProjectSettings::set_order(const String &p_name, int p_order) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	props[p_name].order = p_order;
}

/*
 * Assign order inex for built-in setting, if not already set.
 *
 * @param p_name - The name of the project setting.
 */
void ProjectSettings::set_builtin_order(const String &p_name) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));

	VariantContainer &vc = props[p_name];

	if (vc.order >= NO_BUILTIN_ORDER_BASE) {
		vc.order = last_builtin_order++;
	}
}

/*
 * Check if setting is considered built-in.
 *
 * @param p_name - The name of the project setting.
 *
 * @return - True, if setting is built-in.
 *           False, otherwise.
 */
bool ProjectSettings::is_builtin_setting(const String &p_name) const {
	// Return true because a false negative is worse than a false positive.
	ERR_FAIL_COND_V_MSG(!props.has(p_name), true, vformat("Request for nonexistent project setting: '%s'.", p_name));
	return props[p_name].order < NO_BUILTIN_ORDER_BASE;
}

/*
 * Remove given setting from project setting.
 *
 * @param p_name - The name of the project setting to remove.
 */
void ProjectSettings::clear(const String &p_name) {
	ERR_FAIL_COND_MSG(!props.has(p_name), vformat("Request for nonexistent project setting: '%s'.", p_name));
	props.erase(p_name);
}

/*
 * Save current project settings to `project.godot`, updates last saved time.
 *
 * @return - OK, on success.
 *           Appropriate error code, on failure.
 */
Error ProjectSettings::save() {
	const String path = get_resource_path().path_join("project.godot");

	Error error = save_custom(path);

	if (error == OK) {
		last_save_time = FileAccess::get_modified_time(path);
	}

	return error;
}

/*
 * Check if given value of a given project exists.
 *
 * @param p_setting - The name of the project setting to retrieve.
 *
 * @param p_default_value - The value to return, if the setting is not found.
 *
 * @return - The value of the setting, if found.
 *           The default value, if not found.
 */
Variant ProjectSettings::get_setting(const String &p_setting, const Variant &p_default_value) const {
	if (has_setting(p_setting)) {
		return get(p_setting);
	} else {
		return p_default_value;
	}
}
