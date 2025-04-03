/**************************************************************************/
/*  module_AES.h                                                          */
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

#ifndef MODULE_AES_H
#define MODULE_AES_H

#include <core/object/ref_counted.h>
#include <core/variant/variant.h>

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
#include <openssl/aes.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#endif // __has_include(<openssl/bio.h>)
#endif

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <memory>
#include <sstream>
#include <vector>

class module_AES : public Object {
	GDCLASS(module_AES, Object);

private:
	enum class AESMode { GCM = 0,
		CBC = 1,
		CTR = 2,
		INVALID = -1 };
	AESMode string_to_aes_mode(const String &mode) {
		// Case insensitivity training FREE, TONIGHT ONLY:
		String big_mode = mode.to_upper();
		if (big_mode == "CBC") {
			return AESMode::CBC;
		}
		if (big_mode == "CTR") {
			return AESMode::CTR;
		}
		if (big_mode == "GCM") {
			return AESMode::GCM;
		}
		return AESMode::INVALID;
	}
	void print_openssl_err();
	std::vector<uint8_t> hex_to_bytes(const String &hex);
	String bytes_to_base64(const std::vector<uint8_t> &bytes);
	std::vector<unsigned char> b64_decode(const String &s);

public:
	static void _bind_methods();
	String generate_key(int bytes);
	String import_key(const String &hex_key);
	String encrypt(const String &plaintext, const String &hex_key, const String &mode);
	String decrypt(const String &ciphertext, const String &hex_key, const String &mode);
	static constexpr int AES_GCM_TAG_LEN = 16;
	static constexpr int HEX_BASE = 16;
};

#endif // MODULE_AES_H
