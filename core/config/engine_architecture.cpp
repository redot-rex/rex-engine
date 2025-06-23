/**************************************************************************/
/*  engine_architecture.cpp                                               */
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
 * Returns the CPU architecture name for this engine build.
 */
[[nodiscard]] String Engine::get_architecture_name() const {
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(_M_X64)
	return "x86_64";

#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
	return "x86_32";

#elif defined(__aarch64__) || defined(_M_ARM64) || defined(_M_ARM64EC)
	return "arm64";

#elif defined(__arm__) || defined(_M_ARM)
	return "arm32";

#elif defined(__riscv)
#if __riscv_xlen == 64
	return "rv64";
#else
	return "riscv";
#endif

#elif defined(__powerpc__)
#if defined(__powerpc64__)
	return "ppc64";
#else
	return "ppc";
#endif

#elif defined(__loongarch64)
	return "loongarch64";

#elif defined(__wasm__)
#if defined(__wasm64__)
	return "wasm64";
#elif defined(__wasm32__)
	return "wasm32";
#endif
#endif

	return "unknown";
}
