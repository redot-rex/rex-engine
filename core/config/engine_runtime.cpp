/**************************************************************************/
/*  engine_runtime.cpp                                                    */
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
 * Returns whether or not the engine is embedded inside an editor.
 *
 * @return - True, if the engine is running embedded in an editor.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_embedded_in_editor() const {
	return embedded_in_editor;
}

/*
 * Toggles whether the engine is embedded inside an editor.
 *
 * @param p_enabled - Mark the engine as embedded in an editor or not.
 */
void Engine::set_embedded_in_editor(bool p_enabled) {
	embedded_in_editor = p_enabled;
}

/*
 * Informs the frame server that a synchronization point was reached.
 *
 * @return - True, if the number of server syncs exceeds warning threshold.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::notify_frame_server_synced() {
	frame_server_synced = true;

	// Checks if number of server syncs crosses warning threshold.
	return server_syncs > SERVER_SYNC_FRAME_COUNT_WARNING;
}
