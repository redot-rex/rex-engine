/**************************************************************************/
/*  module_RSA.h                                                          */
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

#ifndef MODULE_RSA_H
#define MODULE_RSA_H

#include <core/crypto/crypto_core.h> // For base64
#include <core/object/ref_counted.h>
#include <core/variant/variant.h>

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
#include <openssl/bio.h> // For module_RSA::b64_decode()
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#endif // #if __has_include(<openssl/bio.h>)
#endif

#include <algorithm>
#include <string>
#include <vector>

class module_RSA : public Object {
	GDCLASS(module_RSA, Object);

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
private:
	EVP_PKEY *privkey;
	EVP_PKEY *pubkey;
#endif // #if __has_include(<openssl/bio.h>)
#endif

public:
	static void _bind_methods();
	module_RSA();
	~module_RSA();
	std::vector<unsigned char> b64_decode(const String &s);
	bool generate_keys(int bits);
	String encrypt(const String &plaintext);
	String decrypt(const String &ciphertext);
};

#endif // MODULE_RSA_H
