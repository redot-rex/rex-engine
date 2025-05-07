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
	ClassDB::bind_method(D_METHOD("encrypt", "plaintext", "self"), &module_RSA::encrypt, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("decrypt", "ciphertext"), &module_RSA::decrypt);
	ClassDB::bind_method(D_METHOD("import_privkey", "privkey"), &module_RSA::import_privkey);
	ClassDB::bind_method(D_METHOD("import_pubkey", "pubkey", "self"), &module_RSA::import_pubkey, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("export_privkey"), &module_RSA::export_privkey);
	ClassDB::bind_method(D_METHOD("export_pubkey", "self"), &module_RSA::export_pubkey, DEFVAL(true));
	ClassDB::bind_method(D_METHOD("sign", "data"), &module_RSA::sign);
	ClassDB::bind_method(D_METHOD("verify", "data", "signature", "self"), &module_RSA::verify, DEFVAL(true));
}

#ifdef __has_include
#if __has_include(<openssl/bio.h>)
module_RSA::module_RSA() :
		privkey(nullptr, EVP_PKEY_free),
		pubkey(nullptr, EVP_PKEY_free),
		server_pubkey(nullptr, EVP_PKEY_free) {}

module_RSA::~module_RSA() {
	privkey.reset();
	pubkey.reset();
	server_pubkey.reset();
}

// FIXME: Remove this when we get CryptoCore::b64_decode() working.
std::vector<unsigned char> module_RSA::b64_decode(const String &s) {
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

bool setup_oaep_sha256(EVP_PKEY_CTX *ctx) {
	/*
	 * Set padding to sha256.
	 */

	if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING) <= 0) {
		print_error("Failed to set RSA OAEP padding.");
		return false;
	}

	// Use SHA-256 as the OAEP digest.
	if (EVP_PKEY_CTX_set_rsa_oaep_md(ctx, EVP_sha256()) <= 0) {
		print_error("Failed to set OAEP digest to SHA-256.");
		return false;
	}

	// Match MGF1 hash to SHA-256 as well (required).
	if (EVP_PKEY_CTX_set_rsa_mgf1_md(ctx, EVP_sha256()) <= 0) {
		print_error("Failed to set MGF1 digest to SHA-256.");
		return false;
	}

	return true;
}

bool module_RSA::generate_keys(int bits) {
	/*
	 * Generate RSA keys based on given bit size.
	 */

	// Check if bits are acceptable
	switch (bits) {
		case 1024:
			print_error("WARNING: Very weak bit-size.");
			break;
		case 2048:
			break;
		case 3072:
			break;
		case 4096:
			break;
		case 8192:
			break;
		default:
			if (bits < 1024 || bits > 8192 || (bits % 1024) != 0) {
				print_error("Invalid key size for RSA. It must be  between 1024 - 8192 and a multiple of 1024.");
				return false;
			}
			return false;
	}

	// Free the keys
	privkey.reset(pubkey.release());
	pubkey.reset();

	// https://docs.openssl.org/3.4/man3/EVP_PKEY_CTX_new/
	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);

	// https://docs.openssl.org/3.4/man3/EVP_PKEY_keygen/
	if (!ctx || EVP_PKEY_keygen_init(ctx) <= 0) {
		// https://docs.openssl.org/3.4/man7/life_cycle-pkey/
		EVP_PKEY_CTX_free(ctx);
		print_error("Key gen context init. failed.");
		return false;
	}

	// https://docs.openssl.org/3.4/man3/EVP_PKEY_CTX_ctrl/
	if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
		// Set param for key size.
		EVP_PKEY_CTX_free(ctx);
		print_error("Failed to set RSA key size.");
		return false;
	}

	// Allocate empty EVP_PKEY struct for storing pub/priv keys.
	EVP_PKEY *tmp_key = nullptr;

	// https://docs.openssl.org/3.4/man3/EVP_PKEY_keygen/
	if (EVP_PKEY_keygen(ctx, &tmp_key) <= 0) {
		// Generate the key.
		EVP_PKEY_CTX_free(ctx);
		print_error("Key gen failed.");
		return false;
	} else {
		privkey.reset(tmp_key);
	}

	pubkey.reset(privkey.get());

	// Free up mem.
	EVP_PKEY_CTX_free(ctx);

	// key successfully generated. Nice! Now go drink some water.
	return true;
}

String module_RSA::encrypt(const String &plaintext, bool self) {
	/*
	 * Encrypts given plaintext.
	 */

	size_t enc_len; // Stores length of encrypted output.

	if ((!pubkey && self) || (!self && !server_pubkey)) {
		// Dude, where's my pubkey?
		print_error("No public key.");
		return "ERR: No pubkey";
	}

	EVP_PKEY *key = self ? pubkey.get() : server_pubkey.get();

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);

	if ((!ctx) || (EVP_PKEY_encrypt_init(ctx) <= 0)) {
		// context + init check.
		EVP_PKEY_CTX_free(ctx);
		print_error("Encryption context init failed.");
		return "ERR: Encryption context init failed.";
	}

	if (!setup_oaep_sha256(ctx)) {
		EVP_PKEY_CTX_free(ctx);
		print_error("Encryption padding failed.");
		return "ERR: Encryption padding failed.";
	}

	// https://docs.openssl.org/3.4/man3/EVP_PKEY_encrypt/
	if (EVP_PKEY_encrypt(ctx, nullptr, &enc_len, (unsigned char *)plaintext.utf8().get_data(), plaintext.length()) <= 0) {
		// Checks if it obtained length of encrypted output.
		EVP_PKEY_CTX_free(ctx);
		print_error("Could not determine encryption length.");
		return "ERR: Could not determine encryption length.";
	}

	std::vector<uint8_t> encrypted(enc_len);

	// Encrypt
	if (EVP_PKEY_encrypt(ctx, encrypted.data(), &enc_len, (unsigned char *)plaintext.utf8().get_data(), plaintext.length()) <= 0) {
		// Checks if it encrypted the plaintext.
		EVP_PKEY_CTX_free(ctx);
		print_error("Failed to encrypt plaintext.");
		return "ERR: Failed to encrypt plaintext";
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
		print_error("No private key.");
		return "ERR: NO PRIVATE KEY";
	}

	size_t enc_len; // Encoding length.
	std::vector<uint8_t> decrypted(0); // This holds the decrypted ciphertext.

	//String encrypted_data = CryptoCore::b64_decode((unsigned char *)ciphertext.data(), ciphertext.size());
	// FIXME: ^ that isn't how you use CryptoCore::b64_decode() -- SEE: core/crypto/crypto_core.cpp:235
	const std::vector<unsigned char> encrypted_data = b64_decode(ciphertext);

	if (encrypted_data.empty()) {
		print_error("EMPTY. DO NOT PASS GO.");
		return "EMPTY ENCRYPTED DATA?";
	}

	EVP_PKEY *key = privkey.get();

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);

	if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0) {
		// check context, init for decryption.
		EVP_PKEY_CTX_free(ctx);
		print_error("Decryption context init failed.");
		return "ERR: Decryption context init failed.";
	}

	if (!setup_oaep_sha256(ctx)) {
		EVP_PKEY_CTX_free(ctx);
		print_error("Decryption padding failed.");
		return "ERR: Decryption padding failed.";
	}

	if (EVP_PKEY_decrypt(ctx, nullptr, &enc_len, encrypted_data.data(), encrypted_data.size()) <= 0) {
		// Get length of encrypted input.
		EVP_PKEY_CTX_free(ctx);
		print_error("Failed to obtain decryption length.");
		return "ERR: Failed to obtain decryption length.";
	}

	decrypted.resize(enc_len);

	if (EVP_PKEY_decrypt(ctx, decrypted.data(), &enc_len, encrypted_data.data(), encrypted_data.size()) <= 0) {
		// This is the actual decryption part.
		unsigned long err_code = ERR_get_error();
		char err_msg[120];
		ERR_error_string_n(err_code, err_msg, sizeof(err_msg));
		EVP_PKEY_CTX_free(ctx);
		print_error(String("Failed to decrypt.") + err_msg);
		return "ERR: failed to decrypt";
	}

	EVP_PKEY_CTX_free(ctx);

	return String(reinterpret_cast<const char *>(decrypted.data()), decrypted.size());
}

void module_RSA::import_privkey(const String &p) {
	/*
	 * Import private key.
	 */

	privkey.reset();

	std::string s = std::string(p.utf8().get_data());
	int s_len = p.utf8().length();

	BIO *bio = BIO_new_mem_buf(s.data(), s_len);

	if (!bio) {
		print_error("PRIV KEY: Failed to create BIO");
		return;
	}

	// https://docs.openssl.org/3.4/man3/PEM_read_bio_PrivateKey
	privkey.reset(PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr));

	if (!privkey) {
		print_error("Failed to read the private key.");
		return;
	}

	pubkey.reset(privkey.get());
}

void module_RSA::import_pubkey(const String &p, bool self) {
	/*
	 * Import public key.
	 */

	// Deal with previously existing key:
	if (self) {
		pubkey.reset();
	} else {
		server_pubkey.reset();
	}

	std::string s = std::string(p.utf8().get_data());
	int s_len = p.utf8().length();

	BIO *bio = BIO_new_mem_buf(s.data(), s_len);

	if (!bio) {
		print_error("PUB KEY: Failed to create BIO.");
		return;
	}

	// https://docs.openssl.org/3.4/man3/PEM_read_bio_PrivateKey
	EVP_PKEY *key = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);

	BIO_free(bio);

	if (!key) {
		print_error("Failed to read the public key.");
		return;
	}

	if (self) {
		pubkey.reset(key);
	} else {
		server_pubkey.reset(key);
	}
}

String module_RSA::export_privkey() {
	/*
	 * Exports private key in the PEM format.
	 */

	if (!privkey) {
		print_error("No private key.");
		return "No private key";
	}

	BIO *bio = BIO_new(BIO_s_mem());

	if (!bio) {
		print_error("Failed to create BIO for export privkey.");
		return "Failed to create BIO for export privkey.";
	}

	// /home/mcdubh/BLOWFISH/rsa/fixed.cpp:232
	if (PEM_write_bio_PrivateKey(bio, privkey.get(), nullptr, nullptr, 0, nullptr, nullptr) < 1) {
		BIO_free(bio);
		print_error("Something went wrong exporting the private key.");
		return "Something went wrong exporting the private key.";
	}

	char *data = nullptr;

	long len = BIO_get_mem_data(bio, &data);

	String pem_privkey(data, len);

	BIO_free(bio);

	return pem_privkey;
}

String module_RSA::export_pubkey(bool self) {
	/*
	 * Exports public key in the PEM format.
	 */

	if ((!pubkey && self) || (!server_pubkey && !self)) {
		print_error("No public key available for export.");
		return "No public key available for export.";
	}

	BIO *bio = BIO_new(BIO_s_mem());

	int res = self ? PEM_write_bio_PUBKEY(bio, pubkey.get()) : PEM_write_bio_PUBKEY(bio, server_pubkey.get());

	if (res < 1) {
		BIO_free(bio);
		print_error("Something went wrong exporting the public key.");
		return "Something went wrong exporting the public key.";
	}

	char *data = nullptr;

	long long len = BIO_get_mem_data(bio, &data);

	String pem_pubkey(data, len);

	BIO_free(bio);

	return pem_pubkey;
}

std::vector<uint8_t> module_RSA::hash(const String &d) {
	/*
	 * Creates a sha-256 hash out of given data.
	 * This is needed to deal with "data too large for key size" issues.
	 */

	// https://docs.openssl.org/3.4/man3/ASN1_item_sign/
	EVP_MD_CTX *ctx = EVP_MD_CTX_new();

	if (!ctx) {
		print_error("hashing context failed to gen.");
		return {};
	}

	// https://docs.openssl.org/3.4/man3/EVP_DigestInit/
	if (EVP_DigestInit(ctx, EVP_sha256()) <= 0) {
		EVP_MD_CTX_free(ctx);
		print_error("hash init failed");
		return {};
	}

	// Yeah, yeah.. We could use `EVP_MAX_MD_SIZE` for modularity, but sha256 is 32 bytes.
	std::vector<uint8_t> hash(32);
	unsigned int hash_len;

	if (EVP_DigestUpdate(ctx, d.get_data(), d.size()) <= 0) {
		// If you don't update the digest, things get real dicey.
		EVP_MD_CTX_free(ctx);
		print_error("Failed to update digest.");
		return {};
	}

	// https://docs.openssl.org/3.4/man3/EVP_DigestInit/
	if (EVP_DigestFinal(ctx, hash.data(), &hash_len) <= 0) {
		EVP_MD_CTX_free(ctx);
		print_error("finalize hash failed.");
		return {};
	}

	hash.resize(hash_len);

	EVP_MD_CTX_free(ctx);

	return hash;
}

String module_RSA::sign(const String &data) {
	// Sign the input data using the private RSA key.
	if (!privkey) {
		print_error("No private key available for signing.");
		return "ERR: No private key";
	}

	if (data.length() == 0) {
		print_error("Empty string.");
		return "ERR: empty string.";
	}

	// Time to hash this out.
	std::vector<uint8_t> hashed_data = hash(data);

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privkey.get(), nullptr); // Create a context for signing.

	if (!ctx || EVP_PKEY_sign_init(ctx) <= 0) {
		EVP_PKEY_CTX_free(ctx);
		print_error("Signing context initialization failed.");
		return "ERR: Signing context initialization failed.";
	}

	if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
		// We need to set the digest to 256.
		EVP_PKEY_CTX_free(ctx);
		print_error("Verification digest type failed.");
		return "Verification digest type failed.";
	}

	// Determine the size of the signature.
	size_t sig_len;
	if (EVP_PKEY_sign(ctx, nullptr, &sig_len, (unsigned char *)hashed_data.data(), hashed_data.size()) <= 0) {
		EVP_PKEY_CTX_free(ctx);
		print_error("Signing length determination failed.");
		return "ERR: Failed to determine signing length.";
	}

	std::vector<uint8_t> signature(sig_len); // Allocate space for the signature.

	// Actually perform the signing operation.
	if (EVP_PKEY_sign(ctx, signature.data(), &sig_len, (unsigned char *)hashed_data.data(), hashed_data.size()) <= 0) {
		EVP_PKEY_CTX_free(ctx);
		print_error("Signing operation failed.");
		return "ERR: Signing failed.";
	}

	EVP_PKEY_CTX_free(ctx); // Clean up context.
	// Return the signature as a Base64 encoded string for portability.
	return CryptoCore::b64_encode_str(signature.data(), signature.size());
}

bool module_RSA::verify(const String &data, const String &signature, bool self) {
	/*
	 * Verify a signature using a pubkey.
	 */

	if ((!pubkey && self) || (!server_pubkey && !self)) {
		print_error("No pubkey available for verification.");
		return false;
	}

	EVP_PKEY *key = self ? pubkey.get() : server_pubkey.get();

	EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(key, nullptr);

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_verify/
	if ((!ctx) || (EVP_PKEY_verify_init(ctx) <= 0)) {
		// context + init check.
		EVP_PKEY_CTX_free(ctx);
		print_error("Verification context init failed.");
		return false;
	}

	if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
		// We need to set the digest to 256.
		EVP_PKEY_CTX_free(ctx);
		print_error("Verification digest type failed.");
		return false;
	}

	// haha hash it out joke, a second time :DDDDDD
	std::vector<uint8_t> hashed_data = hash(data);

	const auto decoded_signature = b64_decode(signature);

	// https://docs.openssl.org/3.2/man3/EVP_PKEY_verify/
	int result = EVP_PKEY_verify(ctx, decoded_signature.data(), decoded_signature.size(), hashed_data.data(), hashed_data.size());

	EVP_PKEY_CTX_free(ctx);

	if (result) {
		return true;
	} else {
		return false;
	}
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

String module_RSA::encrypt(const String &plaintext, bool self) {
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
void module_RSA::import_privkey(const String &p) {
	/*
	 * Non-OpenSSL response. Does nothing.
	 */
}

void module_RSA::import_pubkey(const String &p, bool self) {
	/*
	 * Non-OpenSSL response. Does nothing.
	 */
}

String module_RSA::export_privkey() {
	/*
	 * Non-OpenSSL response. Does nothing.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

String module_RSA::export_pubkey(bool self) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

std::vector<uint8_t> module_RSA::hash(const String &d) {
	/*
	 * Non-OpenSSL response. Returns nothing.
	 */
	return {};
}

String module_RSA::sign(const String &data) {
	/*
	 * Non-OpenSSL response. Returns error.
	 */
	return "Not implemented - Install the OpenSSL library.";
}

bool module_RSA::verify(const String &data, const String &signature, bool self) {
	/*
	 * Non-OpenSSL response. Returns false.
	 */
	return false;
}

#endif // #if __has_include(<openssl/bio.h>)
#endif
