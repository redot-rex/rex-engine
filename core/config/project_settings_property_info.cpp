/**************************************************************************/
/*  project_settings_property_info.cpp                                    */
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

#include "core/templates/rb_set.h"

/*
 * Obtain property lists used by the editor.
 *
 * @param p_list - Pointer to a list to populate with project property
 *                 metadata.
 */
void ProjectSettings::_get_property_list(List<PropertyInfo> *p_list) const {
	if (!p_list) {
		ERR_PRINT(vformat("Null pointer passed to _get_property_list()"));
		return;
	}

	// mutex lock macro via Godot
	_THREAD_SAFE_METHOD_

	RBSet<_VCSort> vclist;
	HashMap<String, Vector<_VCSort>> setting_overrides;

	for (const auto &E : props) {
		const VariantContainer &v = E.value;

		if (v.hide_from_editor) {
			// skips properties hidden from UI.
			continue;
		}

		_VCSort vc{
			.name = E.key,
			.type = v.variant.get_type(),
			.order = v.order
		};

		bool internal = v.internal;
		if (!internal) {
			for (const auto &F : hidden_prefixes) {
				if (vc.name.begins_with(F)) {
					internal = true;
					break;
				}
			}
		}

		// Mark if setting is storage-only or both storage AND visible in editor.
		vc.flags = internal ? (PROPERTY_USAGE_STORAGE | PROPERTY_USAGE_INTERNAL) : (PROPERTY_USAGE_EDITOR | PROPERTY_USAGE_STORAGE);

		// Mark if basic user-facing setting.
		vc.flags |= v.basic ? PROPERTY_USAGE_EDITOR_BASIC_SETTING : 0;

		// Mark if changing setting requires restart.
		vc.flags |= v.restart_if_changed ? PROPERTY_USAGE_RESTART_IF_CHANGED : 0;

		const int dot = vc.name.rfind_char('.');

		// Detects if property is an override.
		if (dot != -1 && !custom_prop_info.has(vc.name)) {
			StringName n = vc.name.substr(0, dot);
			if (props.has(n)) { // Property is an override.
				setting_overrides[n].append(vc);
			} else {
				vclist.insert(vc);
			}
		} else {
			vclist.insert(vc);
		}
	}

	for (const auto &base : vclist) {
		if (custom_prop_info.has(base.name)) {
			PropertyInfo pi = custom_prop_info[base.name];
			pi.name = base.name;
			pi.usage = base.flags;
			p_list->push_back(pi);
		} else {
			p_list->push_back(PropertyInfo(base.type, base.name, PROPERTY_HINT_NONE, "", base.flags));
		}

		if (setting_overrides.has(base.name)) {
			for (const auto &over : setting_overrides.get(base.name)) {
				if (custom_prop_info.has(over.name)) {
					PropertyInfo pi = custom_prop_info[over.name];
					pi.name = over.name;
					pi.usage = over.flags;
					p_list->push_back(pi);
				} else {
					p_list->push_back(PropertyInfo(over.type, over.name, PROPERTY_HINT_NONE, "", over.flags));
				}
			}
		}
	}
}

/*
 * Binds metadata to use in editor/scripting.
 *
 * @param p_info - Directionary containing property metadata fields
 *                 ("name", "type", "hint" (optional), and "hint_string".)
 */
void ProjectSettings::_add_property_info_bind(const Dictionary &p_info) {
	// input key validation
	ERR_FAIL_COND_MSG(!p_info.has("name"), vformat("Missing 'name' field in property dictionary."));
	ERR_FAIL_COND_MSG(!p_info.has("type"), vformat("Missing 'type' field in property dictionary."));

	PropertyInfo pinfo;
	pinfo.name = p_info["name"];

	// validate setting exists.
	ERR_FAIL_COND_MSG(!props.has(pinfo.name), vformat("Attempted to customize unknown property."));

	// Safe cast type to Variant::Type then validate range.
	const int type_int = p_info["type"];
	ERR_FAIL_INDEX_MSG(type_int, Variant::VARIANT_MAX, "Invalid variant type.");

	pinfo.type = static_cast<Variant::Type>(type_int);

	// Hints are optional here.
	if (p_info.has("hint")) {
		pinfo.hint = PropertyHint(p_info["hint"].operator int());
	}

	if (p_info.has("hint_string")) {
		pinfo.hint_string = p_info["hint_string"];
	}

	set_custom_property_info(pinfo);
}

/*
 * Set custom metadata for specific project setting.
 *
 * @param p_info - The PropertyInfo metadata associated with the setting.
 */
void ProjectSettings::set_custom_property_info(const PropertyInfo &p_info) {
	const String &prop_name = p_info.name;
	// Verify property's registration.
	ERR_FAIL_COND_MSG(!props.has(prop_name), "Attempted to set custom property info for unknown setting.");
	custom_prop_info[prop_name] = p_info;
}
