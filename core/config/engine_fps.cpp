/**************************************************************************/
/*  engine_fps.cpp                                                        */
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

#include "servers/rendering/rendering_device.h"

/*
 * Sets the engine's maximum frame rate limit.
 *
 * @param p_fps - The desired maximum frames per second. A value of 0 or less
 * disables the limit.
 */
void Engine::set_max_fps(uint32_t p_fps) {
	// limit is disabled if given FPS is <= 0
	_max_fps = p_fps > 0 ? p_fps : 0;

	RenderingDevice *rd = RenderingDevice::get_singleton();

	if (rd) {
		rd->_set_max_fps(_max_fps);
	}
}

/*
 * Gets the engine's maximum frame rate limit.
 *
 * @return - The current maximum frames per second. Zero indicates no limit.
 */
[[nodiscard]] uint32_t Engine::get_max_fps() const {
	return _max_fps;
}

/*
 * Sets delays for the engine's frame loop for framerate control or throttling.
 *
 * @param p_msec - The delay in milliseconds to apply between frames.
 */
void Engine::set_frame_delay(uint32_t p_msec) {
	_frame_delay = p_msec;
}

/*
 * Gets delays for the engine's frame loop for framerate control or throttling.
 *
 * @return - The delay in milliseconds between frames.
 */
[[nodiscard]] uint32_t Engine::get_frame_delay() const {
	return _frame_delay;
}
