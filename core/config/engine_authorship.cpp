/**************************************************************************/
/*  engine_authorship.cpp                                                 */
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

#include "engine.h"

#include "core/authors.gen.h"
#include "core/donors.gen.h"
#include "core/license.gen.h"
#include "core/redot_authors.gen.h"
#include "core/variant/typed_array.h"

/*
 * Obtains info from given list.
 *
 * @param info_list - A null-terminated array of const char pointers.
 *
 * @return - Array containing all strings from the list, converted to String.
 */
static Array array_from_info(const char *const *info_list) {
	if (info_list == nullptr) {
		ERR_PRINT("null pointer passed.");
		return Array();
	}

	Array arr;

	int arr_size = 0;

	while (info_list[arr_size] != nullptr) {
		arr_size++;
	}

	arr.resize(arr_size);

	for (int i = 0; info_list[i] != nullptr; i++) {
		arr[i] = String::utf8(info_list[i]);
	}

	return arr;
}

/*
 * Convert fixed-len list of C-str to Godot Array.
 *
 * @param info_list - Pointer to an array of const char* strings.
 *
 * @param info_count - Number of elements in given array.
 *
 * @return - An array containing all converted Strings.
 */
static Array array_from_info_count(const char *const *info_list, int info_count) {
	if (info_list == nullptr || info_count <= 0) {
		ERR_PRINT("Null pointer passed or no information passed.");
		return Array();
	}

	Array arr;

	arr.resize(info_count);

	for (int i = 0; i < info_count; i++) {
		arr[i] = String::utf8(info_list[i]);
	}

	return arr;
}

/*
 * Returns a dictionary of Redot contributors, grouped by category.
 *
 * @return - A Dictionary containing contributor arrays categorized by role.
 */
Dictionary Engine::get_author_info() const {
	Dictionary dict;

	dict["lead_developers"] = array_from_info(REDOT_AUTHORS_LEAD_DEVELOPERS);
	dict["project_managers"] = array_from_info(REDOT_AUTHORS_PROJECT_MANAGERS);
	dict["founders"] = array_from_info(REDOT_AUTHORS_FOUNDERS);
	dict["developers"] = array_from_info(REDOT_AUTHORS_DEVELOPERS);

	return dict;
}

/*
 * Returns Godot contributor info from historical sources.
 *
 * @return - A Dictionary containing contributor arrays categorized by role.
 */
Dictionary Engine::get_godot_author_info() const {
	Dictionary dict;

	dict["lead_developers"] = array_from_info(AUTHORS_LEAD_DEVELOPERS);
	dict["project_managers"] = array_from_info(AUTHORS_PROJECT_MANAGERS);
	dict["founders"] = array_from_info(AUTHORS_FOUNDERS);
	dict["developers"] = array_from_info(AUTHORS_DEVELOPERS);

	return dict;
}

/*
 * Returns copyright info for individual components.
 *
 * @return - A TypedArray of Dictionary entries, one for each component.
 */
TypedArray<Dictionary> Engine::get_copyright_info() const {
	TypedArray<Dictionary> components;

	components.resize(COPYRIGHT_INFO_COUNT);

	// Counter for cp_info.
	int i = 0;

	for (const auto &cp_info : COPYRIGHT_INFO) {
		Dictionary component_dict;

		component_dict["name"] = String::utf8(cp_info.name);

		TypedArray<Dictionary> parts;

		parts.resize(cp_info.part_count);

		for (int j = 0; j < cp_info.part_count; j++) {
			auto &cp_part = cp_info.parts[j];

			Dictionary part_dict;

			part_dict["files"] = array_from_info_count(cp_part.files, cp_part.file_count);
			part_dict["copyright"] = array_from_info_count(cp_part.copyright_statements, cp_part.copyright_count);
			part_dict["license"] = String::utf8(cp_part.license);

			parts[j] = part_dict;
		}

		component_dict["parts"] = parts;

		components[i] = component_dict;

		i++;
	}

	return components;
}

/*
 * Returns a dictionary of Redot donors and patrons.
 *
 * @return - Dictionary containing donor and patron categories.
 */
Dictionary Engine::get_donor_info() const {
	Dictionary donors;

	donors["patrons"] = Array();
	donors["platinum_sponsors"] = Array();
	donors["gold_sponsors"] = Array();
	donors["silver_sponsors"] = Array();
	donors["diamond_members"] = Array();
	donors["titanium_members"] = Array();
	donors["platinum_members"] = Array();
	donors["gold_members"] = Array();

	return donors;
}

/*
 * Returns Godot donor info from historical sources.
 *
 * @return - Dictionary containing categorized historical donor information.
 */
Dictionary Engine::get_godot_donor_info() const {
	Dictionary donors;
	donors["patrons"] = array_from_info(DONORS_PATRONS);
	donors["platinum_sponsors"] = array_from_info(DONORS_SPONSORS_PLATINUM);
	donors["gold_sponsors"] = array_from_info(DONORS_SPONSORS_GOLD);
	donors["silver_sponsors"] = array_from_info(DONORS_SPONSORS_SILVER);
	donors["diamond_members"] = array_from_info(DONORS_MEMBERS_DIAMOND);
	donors["titanium_members"] = array_from_info(DONORS_MEMBERS_TITANIUM);
	donors["platinum_members"] = array_from_info(DONORS_MEMBERS_PLATINUM);
	donors["gold_members"] = array_from_info(DONORS_MEMBERS_GOLD);
	return donors;
}
