/**************************************************************************/
/*  engine_timing.cpp                                                     */
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
 * Sets the time scale for the engine.
 *
 * @param p_scale - The time scale factor to apply. (1.0 is normal speed.)
 */
void Engine::set_time_scale(double p_scale) {
	_time_scale = p_scale;
}

/*
 * Sets whether the engine's time scale should be frozen.
 *
 * @param p_frozen - True to freeze time scale.
 */
void Engine::set_freeze_time_scale(bool p_frozen) {
	freeze_time_scale = p_frozen;
}

/*
 * Retrieves the current time scale of the engine.
 *
 * @return - The time scale.
 *           0.0, if time scale is frozen.
 */
[[nodiscard]] double Engine::get_time_scale() const {
	// Returns 0 if frozen.
	return freeze_time_scale ? 0.0 : _time_scale;
}

/*
 * Returns the internal time scale of the engine, independent of freezing.
 *
 * @return - The configured time scale value.
 */
[[nodiscard]] double Engine::get_unfrozen_time_scale() const {
	return _time_scale;
}
