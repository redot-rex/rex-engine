/**************************************************************************/
/*  engine_paths.cpp                                                      */
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
 * Sets path where gameplay recordings are stored.
 *
 * @param p_path - The file system path to store recordings.
 */
void Engine::set_write_movie_path(const String &p_path) {
	write_movie_path = p_path;
}

/*
 * Returns the path where gameplay recordings are stored.
 *
 * @return - The file system path to store recordings.
 */
[[nodiscard]] String Engine::get_write_movie_path() const {
	return write_movie_path;
}

/*
 * Sets directory used to store compiled shader cache files.
 *
 * @param p_path - The file system path to store shader cache files.
 */
void Engine::set_shader_cache_path(const String &p_path) {
	shader_cache_path = p_path;
}

/*
 * Returns the directory used to store compiled shader cache files.
 *
 * return - The file system path used to store shader cache files.
 */
[[nodiscard]] String Engine::get_shader_cache_path() const {
	return shader_cache_path;
}
