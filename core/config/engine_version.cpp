/**************************************************************************/
/*  engine_version.cpp                                                    */
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

#include "core/version.h"

/*
 * Provides version metadata about the compatibility details.
 *
 * @return - A dictionary containing version metadata.
 */
[[nodiscard]] Dictionary Engine::get_godot_compatible_version_info() const {
	Dictionary dict;

	dict["major"] = GODOT_VERSION_MAJOR;
	dict["minor"] = GODOT_VERSION_MINOR;
	dict["patch"] = GODOT_VERSION_PATCH;
	dict["hex"] = GODOT_VERSION_HEX;
	dict["status"] = GODOT_VERSION_STATUS;

	String stringver = String(dict["major"]) + "." + String(dict["minor"]);

	if ((int)dict["patch"] != 0) {
		stringver += "." + String(dict["patch"]);
	}

	// TODO: add godot automated build identification?
	// stringver += "-" + String(dict["status"]) + " (" + String(dict["build"]) + ")";

	stringver += "-" + String(dict["status"]);

	dict["string"] = stringver;

	return dict;
}

/*
 * Provides version metadata about the engine.
 *
 * @return - A dictionary containing version fields (major, minor, patch, hex,
 *           status, build, status_version, hash, timestamp, and a formatted
 *           version string.)
 */
[[nodiscard]] Dictionary Engine::get_version_info() const {
	Dictionary dict;
	dict["major"] = REDOT_VERSION_MAJOR;
	dict["minor"] = REDOT_VERSION_MINOR;
	dict["patch"] = REDOT_VERSION_PATCH;
	dict["hex"] = REDOT_VERSION_HEX;
	dict["status"] = REDOT_VERSION_STATUS;
	dict["build"] = REDOT_VERSION_BUILD;
	dict["status_version"] = REDOT_VERSION_STATUS_VERSION;

	String hash = String(REDOT_VERSION_HASH);
	dict["hash"] = hash.is_empty() ? String("unknown") : hash;

	dict["timestamp"] = REDOT_VERSION_TIMESTAMP;

	String stringver = String(dict["major"]) + "." + String(dict["minor"]);
	if ((int)dict["patch"] != 0) {
		stringver += "." + String(dict["patch"]);
	}
	stringver += "-" + String(dict["status"]);

	if ((int)dict["status_version"] != 0) {
		stringver += "." + String(dict["status_version"]);
	}

	stringver += " (" + String(dict["build"]) + ")";
	dict["string"] = stringver;

	return dict;
}
