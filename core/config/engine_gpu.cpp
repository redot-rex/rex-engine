/**************************************************************************/
/*  engine_gpu.cpp                                                        */
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
 * Indicates whether GPU errors should abort the program.
 *
 * @ return - True, if program will abort on GPU errors.
 *            False, otherwise.
 */
[[nodiscard]] bool Engine::is_abort_on_gpu_errors_enabled() const {
	return abort_on_gpu_errors;
}

/*
 * Indicates whether Vulkan validation layers are enabled for debugging.
 *
 * @return - True, if validation layers are enabled.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_validation_layers_enabled() const {
	return use_validation_layers;
}

/*
 * Indicates whether SPIR-V debug info is enabled.
 *
 * @return - True, if SPIR-V debug info generation is enabled.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_generate_spirv_debug_info_enabled() const {
	return generate_spirv_debug_info;
}

/*
 * Returns the index of the selected GPU for rendering.
 *
 * @return - The GPU index currently selected for rendering.
 */
[[nodiscard]] int32_t Engine::get_gpu_index() const {
	return gpu_idx;
}

/*
 * Indicates whether detailed GPU memory usage tracking is enabled.
 *
 * @return - True, if extra GPU memory tracking is enabled.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_extra_gpu_memory_tracking_enabled() const {
	return extra_gpu_memory_tracking;
}

#if defined(DEBUG_ENABLED) || defined(DEV_ENABLED)
/*
 * Indicates whether GPU breadcrumb tracking is enabled.
 * Assists with crash diagnostics.
 *
 * @return - True, if accurate breadcrumbs tracking is enabled.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_accurate_breadcrumbs_enabled() const {
	return accurate_breadcrumbs;
}
#endif
