/**************************************************************************/
/*  module_Blowfish.cpp                                                   */
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

#include "module_Blowfish.h"

void module_Blowfish::_bind_methods() {
	//ClassDB::bind_method(D_METHOD("generate_key", "bytes"), &module_Blowfish::generate_key);
	//ClassDB::bind_method(D_METHOD("encrypt", "plaintext"), &module_Blowfish::encrypt);
	//ClassDB::bind_method(D_METHOD("decrypt", "ciphertext"), &module_Blowfish::decrypt);
}

module_Blowfish::module_Blowfish() {}
module_Blowfish::~module_Blowfish() {}

#ifdef __has_include
#if __has_include(<openssl/blowfish.h>)
bool module_Blowfish::generate_key(size_t bytes) {
	/*
	 * Setup encryption key plus mode(s).
	 */

	// TODO: Implement option to select other modes, such as CBC?

	// TODO: Implement this.

	// check for keylen. 4 bytes - 56 bytes. (32bit - 448bit)

	// https://docs.openssl.org/3.4/man3/EVP_EncryptInit/
	// int EVP_EncryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv);
	// if(EVP_EncryptInit_ex(ctx, EVP_bf_cfb(), nullptr, key.data(), iv.data()) =! 1)
	// EVP_EncryptInit_ex returns 0 if err. Announce that it fails to init encryption.

	// https://docs.openssl.org/3.4/man3/EVP_bf_cbc
	// for EVP_bf_cfb()

	return true; // keys successfully generated.
}

String module_Blowfish::encrypt(String plaintext) {
	/*
	 * Encrypts given plaintext.
	 */

	// TODO: Implement this.

	// https://docs.openssl.org/3.4/man3/EVP_EncryptInit/
	// int EVP_EncryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
	// below is just a concept, figure this out better later.
	// if (EVP_EncryptUpdate(ctx, std::vector<uint8_t>(encrypted_data), (int)plaintext_length), plaintext.data(), plaintext.size() != 1)
	// returns 0 if err. Failed to encrypt.
	// return encrypted_data

	return plaintext; // PLACEHOLDER
}

String module_Blowfish::decrypt(String ciphertext) {
	/*
	 * Decrypts given plaintext.
	 */

	// TODO: Implement this.

	// https://docs.openssl.org/3.4/man3/EVP_EncryptInit/
	// int EVP_DecryptInit_ex(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type, ENGINE *impl, const unsigned char *key, const unsigned char *iv);
	// if (EVP_DecryptInit_ex() != 1)
	// Need to init decryption first. Returns 0 if fails to.

	// https://docs.openssl.org/3.4/man3/EVP_EncryptInit/
	// int EVP_DecryptUpdate(EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl, const unsigned char *in, int inl);
	// if ( EVP_DecryptUpdate() != 1)
	// Successful decryption returns 1.

	// return decrypted_data
	return ciphertext; // PLACEHOLDER
}

#else

bool module_Blowfish::generate_key(size_t bytes) {
	/*
	 * This just returns false for now.
	 */

	return false;
}

String module_Blowfish::encrypt(String plaintext) {
	/*
	 * Non-OpenSSL subroutine that returns an error.
	 */

	return "Not implemented - Install the OpenSSL Library.";
}

String module_Blowfish::decrypt(String ciphertext) {
	/*
	 * Non-OpenSSL subroutine that returns an err.r
	 */

	return "Not implemented - Install the OpenSSL Library.";
}
#endif
#endif
