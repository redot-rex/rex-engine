/**************************************************************************/
/*  engine_license.cpp                                                    */
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

#include "core/license.gen.h"

/*
 * Returns license name, type, and optionally the license text.
 *
 * @return - A dictionary mapping license names to their corresponding text.
 */
[[nodiscard]] Dictionary Engine::get_license_info() const {
	Dictionary licenses;

	// TODO: This would be nicer, but it would require reworking core/license.gen.h
	/*
	for (const auto &license : LICENSES) {
		licenses[license.name] = license.body;
	}
	*/

	for (int i = 0; i < LICENSE_COUNT; i++) {
		const auto &name = LICENSE_NAMES[i];
		const auto &body = LICENSE_BODIES[i];
		licenses[name] = body;
	}

	return licenses;
}

/*
 * Returns license text.
 *
 * @return - The full license text as a String.
 */
[[nodiscard]] String Engine::get_license_text() const {
	return String(GODOT_LICENSE_TEXT);
}
