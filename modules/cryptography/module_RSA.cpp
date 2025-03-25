/**************************************************************************/
/*  module_RSA.cpp                                                        */
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

#include "module_RSA.h"

void module_RSA::_bind_methods() {
	ClassDB::bind_method(D_METHOD("generate_keys", "bits"), &module_RSA::generate_keys, DEFVAL(2048));
	ClassDB::bind_method(D_METHOD("encrypt", "plaintext"), &module_RSA::encrypt);
	ClassDB::bind_method(D_METHOD("decrypt", "ciphertext"), &module_RSA::decrypt);
}

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
module_RSA::module_RSA() :
		privkey(nullptr), pubkey(nullptr) {}

module_RSA::~module_RSA() {
	ERR_print_errors_fp(stderr);
}

// FIXME: Remove this when we get CryptoCore::b64_decode() working.
std::vector<unsigned char> module_RSA::b64_decode(const String &s) {
	/*
	 * Decodes given b64 as String
	 */

	std::vector<unsigned char> un_b64(s.length());

	// https://docs.openssl.org/3.2/man3/BIO_s_mem/
	BIO *bio = BIO_new_mem_buf(s.utf8().get_data(), s.length()); // Read-only
	BIO *b64 = BIO_new(BIO_f_base64()); // Sets bio to 64 base

	// https://docs.openssl.org/3.2/man3/BIO_push/
	BIO_push(b64, bio); // chain b64 to bio. {

	// https://docs.openssl.org/3.2/man3/BIO_f_base64/
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // We HATESES the newlines.

	// https://docs.openssl.org/3.2/man3/BIO_read/
	int len = BIO_read(b64, un_b64.data(), un_b64.size()); // just grabs len of b64.

	BIO_free_all(b64); // Clean up this filth, kid.

	if (len < 0) {
		// Dire situation.
		ERR_print_errors_fp(stderr);
		return std::vector<unsigned char>();
	}

	un_b64.resize(len);

	return un_b64;
}

bool module_RSA::generate_keys(int bits) {
	/*
	 * Generate RSA keys based on given bit size.
	 */

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_CTX_new/
	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

	if (!ctx) {
		// failed to make context for keys
		ERR_print_errors_fp(stderr);
		return false;
	}

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_keygen/
	if (EVP_PKEY_keygen_init(ctx) <= 0) {
		// attempts to init the pub/priv-keys
		ERR_print_errors_fp(stderr);
		// https://docs.openssl.org/3.2/man7/life_cycle-pkey/
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_CTX_ctrl/
	if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
		// Set param for key size.
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	// Allocate empty EVP_PKEY struct for storing pub/priv keys.
	privkey = EVP_PKEY_new();

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_keygen/
	if (EVP_PKEY_keygen(ctx, &privkey) <= 0) {
		// Generate the key.
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return false;
	}

	pubkey = privkey;

	// Free up mem.
	EVP_PKEY_CTX_free(ctx);

	// key successfully generated. Nice! Now go drink some water.
	return true;
}

String module_RSA::encrypt(const String &plaintext) {
	/*
	 * Encrypts given plaintext.
	 */

	size_t enc_len; // Stores length of encrypted output.

	if (!pubkey) {
		// Dude, where's my pubkey?
		return "ERR: No pubkey";
	}

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, nullptr);

	if ((!ctx) || (EVP_PKEY_encrypt_init(ctx) <= 0)) {
		// context + init check.
		ERR_print_errors_fp(stderr);

		if (ctx) {
			// Memory leak status: Dealt with.
			EVP_PKEY_CTX_free(ctx);
		}

		return "ERR: context issue";
	}

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_encrypt/
	if (EVP_PKEY_encrypt(ctx, nullptr, &enc_len, (unsigned char *)plaintext.utf8().get_data(), plaintext.length()) <= 0) {
		// Checks if it obtained length of encrypted output.
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return "ERR: len issue (enc)";
	}

	std::vector<uint8_t> encrypted(enc_len);
	// Encrypt
	if (EVP_PKEY_encrypt(ctx, encrypted.data(), &enc_len, (unsigned char *)plaintext.utf8().get_data(), plaintext.length()) <= 0) {
		// Checks if it encrypted the plaintext.
		ERR_print_errors_fp(stderr);
		EVP_PKEY_CTX_free(ctx);
		return "ERR: didn't encrypt plaintext";
	}

	// We can free up the context now.
	EVP_PKEY_CTX_free(ctx);

	String b64_out = CryptoCore::b64_encode_str((unsigned char *)encrypted.data(), encrypted.size());

	return b64_out;
}

String module_RSA::decrypt(const String &ciphertext) {
	/*
	 * Decrypts given ciphertext.
	 */

	if (!privkey) {
		return "ERR: NO PRIVATE KEY";
	}

	size_t enc_len; // Encoding length.
	std::vector<uint8_t> decrypted(0); // This holds the decrypted ciphertext.

	//String encrypted_data = CryptoCore::b64_decode((unsigned char *)ciphertext.data(), ciphertext.size());
	// FIXME: ^ that isn't how you use CryptoCore::b64_decode() -- SEE: core/crypto/crypto_core.cpp:235
	const std::vector<unsigned char> &encrypted_data = b64_decode(ciphertext);

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey, nullptr);

	if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0) {
		// check context, init for decryption.
		ERR_print_errors_fp(stderr);

		if (ctx) {
			EVP_PKEY_CTX_free(ctx);
		}

		return "ERR: Failed to init";
	}

	if (EVP_PKEY_decrypt(ctx, nullptr, &enc_len, encrypted_data.data(), encrypted_data.size()) <= 0) {
		// Get length of encrypted input.
		ERR_print_errors_fp(stderr);

		if (ctx) {
			EVP_PKEY_CTX_free(ctx);
		}

		return "ERR: len issue.";
	}

	decrypted.resize(enc_len);

	if (EVP_PKEY_decrypt(ctx, decrypted.data(), &enc_len, encrypted_data.data(), encrypted_data.size()) <= 0) {
		// This is the actual decryption part.
		ERR_print_errors_fp(stderr);

		EVP_PKEY_CTX_free(ctx);

		return "ERR: failed to decrypt";
	}

	EVP_PKEY_CTX_free(ctx);

	return String(std::string({ decrypted.begin(), decrypted.end() }).c_str());
	//return String(reinterpret_cast<const char*>(decrypted.data()), decrypted.size());
}

#else

module_RSA::module_RSA() {}
module_RSA::~module_RSA() {}

std::vector<unsigned char> module_RSA::b64_decode(const String &s) {
	/*
	 * Non-OpenSSL response. Returns blank std::vector<unsigned char>.
	 */

	return std::vector<unsigned char>(0);
}

bool module_RSA::generate_keys(int bits) {
	/*
	 * Non-OpenSSL response. Returns false.
	 */

	return false;
}

String module_RSA::encrypt(const String &plaintext) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */

	return "Not implemented - Install the OpenSSL Library.";
}

String module_RSA::decrypt(const String &ciphertext) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */

	return "Not implemented - Install the OpenSSL Library.";
}

#endif // #if __has_include(<openssl/bio.h>)
#endif
