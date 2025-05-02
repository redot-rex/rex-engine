/**************************************************************************/
/*  module_AES.cpp                                                        */
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

#include "module_AES.h"

void module_AES::_bind_methods() {
	ClassDB::bind_method(D_METHOD("generate_key", "bytes"), &module_AES::generate_key, DEFVAL(16));
	ClassDB::bind_method(D_METHOD("import_key", "key"), &module_AES::import_key);
	ClassDB::bind_method(D_METHOD("encrypt", "plaintext", "key", "mode"), &module_AES::encrypt, DEFVAL("GCM"));
	ClassDB::bind_method(D_METHOD("decrypt", "ciphertext", "key", "mode"), &module_AES::decrypt, DEFVAL("GCM"));
}

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
void module_AES::print_openssl_err() {
	/*
	 * Capture OpenSSL errors, prints them.
	 */

	unsigned long err_code;
	while ((err_code = ERR_get_error())) {
		char err_msg[255];
		ERR_error_string_n(err_code, err_msg, sizeof(err_msg));
		print_error(String(err_msg));
	}
}

// TODO: I have this idea. I should make a "support.h" file and include this
// both this and the openssl error handler, plus the other support functions.
// This would massively reduce redundant code... plus it would look less sloppy
// in general.
struct EVP_CTX_Deleter {
	/*
	 * RAII wrapper for EVP_CIPHER_CTX
	 * Some nice guy in IRC suggested this, but asked to be anonymous.
	 * ty "irc guy".
	 */

	void operator()(EVP_CIPHER_CTX *ctx) const {
		if (ctx) {
			EVP_CIPHER_CTX_free(ctx);
		}
	}
};

std::vector<uint8_t> module_AES::hex_to_bytes(const String &hex) {
	/*
	 * Converts a hexadecimal string into a byte vector.
	 */

	if (hex.length() % 2 != 0) {
		print_error("Hex string must have an even number of characters.");
		return {};
	}

	std::vector<uint8_t> bytes;

	bytes.reserve(hex.length() / 2);

	for (int i = 0; i < hex.length(); i += 2) {
		String byte_str = hex.substr(i, 2);
		char byte = static_cast<char>(std::stoi(byte_str.utf8().get_data(), nullptr, HEX_BASE));
		bytes.push_back(static_cast<uint8_t>(byte));
	}

	return bytes;
}

String module_AES::bytes_to_base64(const std::vector<uint8_t> &bytes) {
	/*
	 * Convert given byte vector to b64 String.
	 */

	if (bytes.empty()) {
		print_error("base64 length is zero");
		return "";
	}

	// https://docs.openssl.org/3.4/man3/BIO_s_mem/
	BIO *bio, *b64;
	BUF_MEM *buffer_ptr;

	// https://docs.openssl.org/3.4/man3/BIO_s_mem/
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());

	// https://docs.openssl.org/3.4/man3/BIO_push/
	bio = BIO_push(b64, bio);

	// https://docs.openssl.org/3.4/man3/BIO_f_base64/
	BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); // no newlines here.

	// https://docs.openssl.org/3.4/man3/BIO_s_datagram/
	BIO_write(bio, bytes.data(), bytes.size());

	// https://docs.openssl.org/3.4/man3/BIO_f_base64/
	BIO_flush(bio); // finalizes.

	// https://docs.openssl.org/3.4/man3/BIO_s_mem/
	BIO_get_mem_ptr(bio, &buffer_ptr);

	String base64_str = String::utf8(buffer_ptr->data, buffer_ptr->length);

	BIO_free_all(bio); // CLEAN IT UP, KID

	return base64_str;
}

// FIXME: Remove this when we get CryptoCore::b64_decode() working.
std::vector<unsigned char> module_AES::b64_decode(const String &s) {
	/*
	 * Decodes given b64 as String
	 */

	std::string padded_s = s.utf8().get_data();
	while (padded_s.length() % 4 != 0) {
		// Stupid hack to fix lack of padding.
		padded_s += '=';
	}

	std::vector<unsigned char> un_b64(padded_s.length());

	if (padded_s.length() <= 0) {
		print_error("base64 length is zero");
		return {};
	}

	// https://docs.openssl.org/3.4/man3/BIO_s_mem/
	BIO *bio = BIO_new_mem_buf(padded_s.data(), padded_s.length()); // Read-only
	BIO *b64 = BIO_new(BIO_f_base64()); // Sets bio to 64 base

	// https://docs.openssl.org/3.4/man3/BIO_push/
	BIO_push(b64, bio); // chain b64 to bio.

	// https://docs.openssl.org/3.4/man3/BIO_f_base64/
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // We HATESES the newlines.

	// https://docs.openssl.org/3.4/man3/BIO_read/
	int len = BIO_read(b64, un_b64.data(), un_b64.size()); // just grabs len of b64.

	BIO_free_all(b64); // Clean up this filth, kid.

	if (len < 0) {
		// Dire situation.
		print_error("base64 decode failed.");
		return {}; //std::vector<unsigned char>();
	}

	un_b64.resize(len);

	return un_b64;
}

String module_AES::generate_key(int bytes) {
	/*
	 * Generate AES key with given bytes.
	 */

	switch (bytes) {
		case 16:
		case 24:
		case 32:
			break;
		default:
			print_error("Invalid key size. Must be 16, 24, or 32 bytes.");
			return "";
	}

	std::vector<uint8_t> key(bytes);
	if (RAND_bytes(key.data(), bytes) <= 0) {
		print_error("Failed to generate random bytes for key.");
		return "";
	}

	String hex_key;
	// I PUT A SPELL ON YOU.
	for (const auto &byte : key) {
		// CAUSE YOU'RE MINE.
		hex_key += vformat("%02x", byte);
	}

	if (hex_key.length() != bytes * 2) {
		print_error("Unexpected hex key length.");
		return "";
	}

	if (hex_key.length() != bytes * 2) {
		// Do "not take randomness for granted" - OpenSSL doc: man3/RAND_bytes
		return generate_key(bytes);
	}
	return hex_key;
}

String module_AES::import_key(const String &hex_key) {
	/*
	 * Import given key.
	 */

	if (!hex_key.is_valid_hex_number(false)) {
		// Check if valid hex.
		print_error("Invalid key, must be in hex format.");
		return "";
	}

	if (hex_key.length() != 32 && hex_key.length() != 48 && hex_key.length() != 64) {
		// Check if valid length.
		print_error("Invalid key length. Must be 32, 48, 64 hex chars. (16,24,32 bytes)");
		return "";
	}

	return hex_key.to_upper();
}

String module_AES::encrypt(const String &plaintext, const String &hex_key, const String &mode) {
	/*
	 * Encrypt given plaintext into ciphertext, converts ciphertext into
	 * base64, using given key + mode.
	 */

	int len = 0;
	int ciphertext_len = 0;

	// We need to change the plaintext into a PackedByteArray.
	PackedByteArray plaintext_bytes;
	const char *utf8 = plaintext.utf8().get_data();
	plaintext_bytes.resize(strlen(utf8));
	memcpy(plaintext_bytes.ptrw(), utf8, strlen(utf8));

	// De-hex that key.
	std::vector<uint8_t> key = hex_to_bytes(hex_key);
	if (key.empty()) {
		print_error("YOUR KEY IS EMPTY.");
		return "";
	}

	// Convert to proper mode.
	const EVP_CIPHER *cipher = nullptr;
	AESMode mode_string = string_to_aes_mode(mode);

	switch (mode_string) {
		case AESMode::CBC:
			cipher = (key.size() == 16)	 ? EVP_aes_128_cbc()
					: (key.size() == 24) ? EVP_aes_192_cbc()
										 : EVP_aes_256_cbc();
			break;
		case AESMode::CTR:
			cipher = (key.size() == 16)	 ? EVP_aes_128_ctr()
					: (key.size() == 24) ? EVP_aes_192_ctr()
										 : EVP_aes_256_ctr();
			break;
		case AESMode::GCM:
			cipher = (key.size() == 16)	 ? EVP_aes_128_gcm()
					: (key.size() == 24) ? EVP_aes_192_gcm()
										 : EVP_aes_256_gcm();
			break;
		default:
			print_error("Invalid cipher mode.");
			return "";
	}

	// Let's generate the IVs
	int iv_length = EVP_CIPHER_iv_length(cipher);
	std::vector<uint8_t> iv(iv_length);
	if (iv_length > 0 && !RAND_bytes(iv.data(), iv_length)) {
		print_error("IV generation failed.");
		return "";
	}

	// Now for the context.
	std::unique_ptr<EVP_CIPHER_CTX, EVP_CTX_Deleter> ctx(EVP_CIPHER_CTX_new());

	if (!ctx) {
		print_error("Encryption context failed to generate.");
		return "";
	}

	if (!EVP_EncryptInit_ex(ctx.get(), cipher, nullptr, key.data(), iv.data())) {
		print_openssl_err();
		return "";
	}

	switch (mode_string) {
		case AESMode::CBC:
			// CBC is special and gets PKCS7 padding.
			EVP_CIPHER_CTX_set_padding(ctx.get(), 1);
			break;
		default:
			// The normie, unspecial ones don't.
			EVP_CIPHER_CTX_set_padding(ctx.get(), 0);
			break;
	}

	// We're going to need to change that String into a utf-8 byte vector.
	std::vector<uint8_t> plaintext_vec(plaintext_bytes.ptr(), plaintext_bytes.ptr() + plaintext_bytes.size());
	std::vector<uint8_t> ciphertext(plaintext_vec.size() + EVP_CIPHER_block_size(cipher));

	if (!EVP_EncryptUpdate(ctx.get(), ciphertext.data(), &len, plaintext_bytes.ptr(), plaintext_bytes.size())) {
		// Welcome to the magic that encrypts the plaintext.
		print_openssl_err();
		return "";
	}

	ciphertext_len = len;

	switch (mode_string) {
		case AESMode::CBC:
		case AESMode::GCM:
			// Ok, I lied. GCM isn't a normie. CTR is though.
			if (!EVP_EncryptFinal_ex(ctx.get(), ciphertext.data() + len, &len)) {
				print_openssl_err();
				return "";
			}
			ciphertext_len += len;
			break;
		default:
			break;
	}

	std::vector<uint8_t> auth_tag(AES_GCM_TAG_LEN);

	switch (mode_string) {
		case AESMode::GCM:
			// GCM has very special needs. (The auth tag.)
			if (!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_GCM_GET_TAG, AES_GCM_TAG_LEN, auth_tag.data())) {
				print_openssl_err();
				return "";
			}
			break;
		default:
			break;
	}

	// Let's make this reasonable.
	ciphertext.resize(ciphertext_len);

	// Altogether now.
	std::vector<uint8_t> final_output;
	final_output.insert(final_output.end(), iv.begin(), iv.end());
	final_output.insert(final_output.end(), ciphertext.begin(), ciphertext.end());

	switch (mode_string) {
		case AESMode::GCM:
			// GCM, our special boy, don't forget your auth tag.
			final_output.insert(final_output.end(), auth_tag.begin(), auth_tag.end());
			break;
		default:
			break;
	}

	// Return it in base64.
	return bytes_to_base64(final_output);
}

String module_AES::decrypt(const String &ciphertext, const String &hex_key, const String &mode) {
	/*
	 * Decrypts the given ciphertext (in base64) using the provided key and mode.
	 */

	int len = 0;
	int plaintext_len = 0;

	// Convert hex key to a byte vector
	std::vector<uint8_t> key = hex_to_bytes(hex_key);
	if (key.empty()) {
		return "Error: Invalid key format.";
	}

	// Decode Base64 ciphertext
	std::vector<unsigned char> raw_ciphertext_bytes = b64_decode(ciphertext);
	if (raw_ciphertext_bytes.empty()) {
		return "Error: Invalid Base64 ciphertext.";
	}

	// Convert to PBA
	PackedByteArray ciphertext_bytes;
	ciphertext_bytes.resize(raw_ciphertext_bytes.size());
	memcpy(ciphertext_bytes.ptrw(), raw_ciphertext_bytes.data(), raw_ciphertext_bytes.size());

	// Convert to proper mode.
	const EVP_CIPHER *cipher = nullptr;
	AESMode mode_string = string_to_aes_mode(mode);

	switch (mode_string) {
		case AESMode::CBC:
			cipher = (key.size() == 16)	 ? EVP_aes_128_cbc()
					: (key.size() == 24) ? EVP_aes_192_cbc()
										 : EVP_aes_256_cbc();
			break;
		case AESMode::CTR:
			cipher = (key.size() == 16)	 ? EVP_aes_128_ctr()
					: (key.size() == 24) ? EVP_aes_192_ctr()
										 : EVP_aes_256_ctr();
			break;
		case AESMode::GCM:
			cipher = (key.size() == 16)	 ? EVP_aes_128_gcm()
					: (key.size() == 24) ? EVP_aes_192_gcm()
										 : EVP_aes_256_gcm();
			break;
		default:
			print_error("Invalid cipher mode.");
			return "";
	}

	// Extract IV from ciphertext
	long int iv_length = EVP_CIPHER_iv_length(cipher);

	if (ciphertext_bytes.size() < iv_length) {
		print_error("Ciphertext is too short.");
		return "";
	}

	std::vector<uint8_t> iv(ciphertext_bytes.ptr(), ciphertext_bytes.ptr() + iv_length);

	std::vector<uint8_t> auth_tag(AES_GCM_TAG_LEN);

	switch (mode_string) {
		case AESMode::GCM:
			if (ciphertext_bytes.size() < iv_length + AES_GCM_TAG_LEN) {
				print_error("ciphertext too short for GCM.");
				return "";
			}
			// GCM tag is at the end of the text.
			auth_tag.assign(ciphertext_bytes.ptr() + ciphertext_bytes.size() - AES_GCM_TAG_LEN, ciphertext_bytes.ptr() + ciphertext_bytes.size());
			// Then remove the tag from the ciphertext.
			ciphertext_bytes.resize(ciphertext_bytes.size() - AES_GCM_TAG_LEN);
			break;
		default:
			break;
	}

	// Oh yeah, don't forget to remove the IV from ciphertext
	PackedByteArray sliced;
	// This has to be done because PackedByteArray.subarray() doesn't exit
	// within the context of Redot (at the moment.)
	sliced.resize(ciphertext_bytes.size() - iv_length);
	memcpy(sliced.ptrw(), ciphertext_bytes.ptr() + iv_length, sliced.size());
	ciphertext_bytes = sliced;

	// Context needed.
	std::unique_ptr<EVP_CIPHER_CTX, EVP_CTX_Deleter> ctx(EVP_CIPHER_CTX_new());

	if (!ctx) {
		print_error("Decryption context failed to generate.");
		return "";
	}

	if (!EVP_DecryptInit_ex(ctx.get(), cipher, nullptr, key.data(), iv.data())) {
		print_openssl_err();
		return "";
	}

	switch (mode_string) {
		case AESMode::CBC:
			// CBC is special and gets PKCS7 padding.
			EVP_CIPHER_CTX_set_padding(ctx.get(), 1);
			break;
		default:
			// The normie, unspecial ones don't.
			EVP_CIPHER_CTX_set_padding(ctx.get(), 0);
			break;
	}

	switch (mode_string) {
		case AESMode::GCM:
			// Don't forget GCM is special and needs an auth tag!
			if (!EVP_CIPHER_CTX_ctrl(ctx.get(), EVP_CTRL_AEAD_SET_TAG, AES_GCM_TAG_LEN, auth_tag.data())) {
				print_openssl_err();
				return "";
			}
			break;
		default:
			break;
	}

	// Alloc. buffer for decrypted plaintext.
	std::vector<uint8_t> plaintext(ciphertext_bytes.size());

	if (!EVP_DecryptUpdate(ctx.get(), plaintext.data(), &len, ciphertext_bytes.ptr(), ciphertext_bytes.size())) {
		print_openssl_err();
		return "";
	}

	plaintext_len = len;

	if (!EVP_DecryptFinal_ex(ctx.get(), plaintext.data() + len, &len)) {
		// Finalize the decryption process.
		print_openssl_err();
		return "";
	}

	plaintext_len += len;

	plaintext.resize(plaintext_len);

	return String::utf8((const char *)plaintext.data(), plaintext.size());
}

#else

void module_AES::print_openssl_err() {
	/*
	 * Non-OpenSSL response. Does nothing.
	 */
}

std::vector<uint8_t> module_AES::hex_to_bytes(const String &hex) {
	/*
	 * Non-OpenSSL response. Returns nothing.
	 */
	return {};
}

String module_AES::bytes_to_base64(const std::vector<uint8_t> &bytes) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

std::vector<unsigned char> module_AES::b64_decode(const String &s) {
	/*
	 * Non-OpenSSL response. Returns nothing.
	 */
	return {};
}

String module_AES::generate_key(int bytes) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

String module_AES::import_key(const String &hex_key) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

String module_AES::encrypt(const String &plaintext, const String &hex_key, const String &mode) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

String module_AES::decrypt(const String &ciphertext, const String &hex_key, const String &mode) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

#endif // #if __has_include(<openssl/bio.h>)
#endif
