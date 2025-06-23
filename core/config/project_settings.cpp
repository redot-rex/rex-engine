/**************************************************************************/
/*  project_settings.cpp                                                  */
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

#include "core/variant/typed_array.h"

/*
 * Registers various functions that makes them accessible to the engine's
 * scripting API.
 */
void ProjectSettings::_bind_methods() {
	ClassDB::bind_method(D_METHOD("has_setting", "name"), &ProjectSettings::has_setting);
	ClassDB::bind_method(D_METHOD("set_setting", "name", "value"), &ProjectSettings::set_setting);
	ClassDB::bind_method(D_METHOD("get_setting", "name", "default_value"), &ProjectSettings::get_setting, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("get_setting_with_override", "name"), &ProjectSettings::get_setting_with_override);
	ClassDB::bind_method(D_METHOD("get_global_class_list"), &ProjectSettings::get_global_class_list);
	ClassDB::bind_method(D_METHOD("get_setting_with_override_and_custom_features", "name", "features"), &ProjectSettings::get_setting_with_override_and_custom_features);
	ClassDB::bind_method(D_METHOD("set_order", "name", "position"), &ProjectSettings::set_order);
	ClassDB::bind_method(D_METHOD("get_order", "name"), &ProjectSettings::get_order);
	ClassDB::bind_method(D_METHOD("set_initial_value", "name", "value"), &ProjectSettings::set_initial_value);
	ClassDB::bind_method(D_METHOD("set_as_basic", "name", "basic"), &ProjectSettings::set_as_basic);
	ClassDB::bind_method(D_METHOD("set_as_internal", "name", "internal"), &ProjectSettings::set_as_internal);
	ClassDB::bind_method(D_METHOD("add_property_info", "hint"), &ProjectSettings::_add_property_info_bind);
	ClassDB::bind_method(D_METHOD("set_restart_if_changed", "name", "restart"), &ProjectSettings::set_restart_if_changed);
	ClassDB::bind_method(D_METHOD("clear", "name"), &ProjectSettings::clear);
	ClassDB::bind_method(D_METHOD("localize_path", "path"), &ProjectSettings::localize_path);
	ClassDB::bind_method(D_METHOD("globalize_path", "path"), &ProjectSettings::globalize_path);
	ClassDB::bind_method(D_METHOD("save"), &ProjectSettings::save);
	ClassDB::bind_method(D_METHOD("load_resource_pack", "pack", "replace_files", "offset"), &ProjectSettings::load_resource_pack, DEFVAL(true), DEFVAL(0));

	ClassDB::bind_method(D_METHOD("save_custom", "file"), &ProjectSettings::_save_custom_bnd);

	ADD_SIGNAL(MethodInfo("settings_changed"));
}

/*
 * ProjectSettings constructor for loading settings from specified path,
 * does not register defaults/modify singleton.
 *
 * @param p_path - The file path to load the project settings from.
 */
ProjectSettings::ProjectSettings(const String &p_path) {
	// Attempt to load from specified file.
	const Error err = load_custom(p_path);

	if (err == OK) {
		resource_path = p_path.get_base_dir();
		project_loaded = true;
	} else {
		ERR_PRINT(vformat("Could not load project settings. Error code %d.", err));
	}
}

/*
 * ProjectSettings deconstructor.
 */
ProjectSettings::~ProjectSettings() {
	if (singleton == this) {
		singleton = nullptr;
	}
}
