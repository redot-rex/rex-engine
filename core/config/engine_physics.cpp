/**************************************************************************/
/*  engine_physics.cpp                                                    */
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
 * Sets the physics ticks per second used for fixed-step simulation.
 *
 * @param p_ips - The number of physics ticks per second. Must be greater than
 * 0.
 */
void Engine::set_physics_ticks_per_second(int p_ips) {
	ERR_FAIL_COND_MSG(p_ips <= 0, "Engine iterations per second must be greater than 0.");
	ips = p_ips;
}

/*
 * Returns the jitter fix threshold for smoother physics interpolation.
 *
 * @return - The jitter fix threshold value.
 */
[[nodiscard]] double Engine::get_physics_jitter_fix() const {
	return physics_jitter_fix;
}

/*
 * Returns the physics ticks per second used for fixed-step simulation.
 *
 * @return - The number of physics ticks per second.
 */
[[nodiscard]] int Engine::get_physics_ticks_per_second() const {
	return ips;
}

/*
 * Sets max number of physics steps that can run in a single frame
 * to prevent spiral of death.
 *
 * @param p_max_physics_steps - The maximum allowed physics steps per frame.
 *                              Must be greater than 0.
 */
void Engine::set_max_physics_steps_per_frame(int p_max_physics_steps) {
	ERR_FAIL_COND_MSG(p_max_physics_steps <= 0, "Maximum number of physics steps per frame must be greater than 0.");
	max_physics_steps_per_frame = p_max_physics_steps;
}

/*
 * Returns physics steps that can be ran in a single frame.
 *
 * @return - The maximum allowed physics steps per frame.
 */
[[nodiscard]] int Engine::get_max_physics_steps_per_frame() const {
	return max_physics_steps_per_frame;
}

/*
 * Set the jitter fix threshold for smoother physics interpolation.
 * A threshold of 0 disables jitter fixing.
 *
 * @param p_threshold - The jitter fix threshold value. Values less than zero
 *                      will be clamped to zero.
 */
void Engine::set_physics_jitter_fix(double p_threshold) {
	if (p_threshold < 0) {
		p_threshold = 0;
	}

	physics_jitter_fix = p_threshold;
}
