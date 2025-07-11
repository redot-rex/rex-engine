/**************************************************************************/
/*  engine_debug.cpp                                                      */
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

/*
 * Toggles whether logs are printed to stdout.
 *
 * @param p_enabled - If true, enables printing logs to stdout.
 */
void Engine::set_print_to_stdout(bool p_enabled) {
	CoreGlobals::print_line_enabled = p_enabled;
}

/*
 * Checks if logs are printing to stdout.
 *
 * @return - True if printing to stdout is enabled.
 *           False if otherwise.
 */
[[nodiscard]] bool Engine::is_printing_to_stdout() const {
	return CoreGlobals::print_line_enabled;
}

/*
 * Toggles printing error messages to stdout.
 *
 * @param p_enabled - If true, enables printing errors to stdout.
 */
void Engine::set_print_error_messages(bool p_enabled) {
	CoreGlobals::print_error_enabled = p_enabled;
}

/*
 * Checks if err are printing to stdout.
 *
 * @return - True if printing to stdout is enabled.
 *           False if otherwise.
 */
[[nodiscard]] bool Engine::is_printing_error_messages() const {
	return CoreGlobals::print_error_enabled;
}

/*
 * Prints given header string.
 *
 * @param p_string - The header string to print.
 */
void Engine::print_header(const String &p_string) const {
	if (_print_header) {
		print_line(p_string);
	}
}

/*
 * Prints formatted header lines with styling or context.
 *
 * @param p_string - The styled header String to print.
 */
void Engine::print_header_rich(const String &p_string) const {
	if (_print_header) {
		print_line_rich(p_string);
	}
}
