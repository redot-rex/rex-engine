/**************************************************************************/
/*  crypto_core.cpp                                                       */
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

#include "crypto_core.h"

#include "core/os/os.h"

#include <mbedtls/base64.h>
#if MBEDTLS_VERSION_MAJOR >= 3
#include <mbedtls/compat-2.x.h>
#endif

// TODO: Make functions to allow for faster hash reuse.
//	   Otherwise, constantly init();finish(); calls.

// Random Generator

/*
 * Constructor for Deterministic Random Bit Generator (DRBG.)
 */
CryptoCore::RandomGenerator::RandomGenerator() {
	// Initialize contexts.
	mbedtls_entropy_init(&entropy);
	mbedtls_ctr_drbg_init(&ctx);

	// Extend entry source using _entropy_poll.
	int ret = mbedtls_entropy_add_source(
			&entropy,
			&CryptoCore::RandomGenerator::_entropy_poll, // pulls from OS layer entropy.
			nullptr,
			256,
			MBEDTLS_ENTROPY_SOURCE_STRONG);

	if (ret != 0) {
		ERR_PRINT("Failed to add entropy source.");
		return;
	}
}

/*
 * DRBG destructor.
 */
CryptoCore::RandomGenerator::~RandomGenerator() noexcept {
	mbedtls_ctr_drbg_free(&ctx);
	mbedtls_entropy_free(&entropy);
}

/*
 * Polls for entropy obtained from the OS.
 *
 * @param p_data - context pointer.
 *
 * @param r_buffer - Buffer to fill with entropy.
 *
 * @param p_len - Requested number of bytes.
 *
 * @param r_len - Pointer to store actual number of bytes written.
 *
 * @return - 0, on success.
 *           MBEDTLS_ERR_ENTROPY_SOURCE_FAILED, on failure.
 */
int CryptoCore::RandomGenerator::_entropy_poll(void *p_data, unsigned char *r_buffer, size_t p_len, size_t *r_len) {
	*r_len = 0;

	// Attempt to fill buffer with entropy from OS.
	Error err = OS::get_singleton()->get_entropy(r_buffer, p_len);

	if (err != OK) {
		ERR_PRINT("Failed to obtain entropy source.");
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}

	*r_len = p_len;

	return 0;
}

/*
 * Initializes Deterministic Random Bit Generator with entropy.
 *
 * @return - OK, on success.
 *           FAILED, if seeding context fails.
 */
Error CryptoCore::RandomGenerator::init() {
	// Seed the CTR-DRBG context with entropy.
	int ret = mbedtls_ctr_drbg_seed(
			&ctx,
			mbedtls_entropy_func,
			&entropy,
			nullptr,
			0);

	if (ret != 0) {
		ERR_PRINT("CryptoCore::RandomGenerator::init failed.");
		return FAILED;
	}

	return OK;
}

/*
 * Fills given buffer with give amount of random bytes
 *
 * @param r_buffer - Pointer to the buffer to be filled.
 *
 * @param p_bytes - Number of random bytes to generate.
 *
 * @return - OK, if success.
 *           FAILED, if buffer is null or generation fails.
 */
Error CryptoCore::RandomGenerator::get_random_bytes(uint8_t *r_buffer, size_t p_bytes) {
	if (!r_buffer) {
		ERR_PRINT("Null buffer passed to get_random_bytes.");
		return FAILED;
	}

	// Attempt to fill buffer with random bytes.
	int ret = mbedtls_ctr_drbg_random(&ctx, r_buffer, p_bytes);

	if (ret != 0) {
		ERR_PRINT("CryptoCore::RandomGenerator::get_random_bytes failed.");
		return FAILED;
	}

	return OK;
}

// MD5

/*
 * Constructor for MD5 hash context.
 */
CryptoCore::MD5Context::MD5Context() {
	mbedtls_md5_init(&ctx);
}

/*
 * Destructor for MD5 hash context.
 */
CryptoCore::MD5Context::~MD5Context() {
	mbedtls_md5_free(&ctx);
}

/*
 * Begins a md5 hash operation.
 *
 * @return - OK, on success.
 *           FAILED, on initialization failed.
 */
Error CryptoCore::MD5Context::start() {
	// Well, it's true.
	ERR_PRINT("MD5 is a weak digest and should only be used for non-security purposes.");

	int ret = mbedtls_md5_starts_ret(&ctx);

	if (ret != 0) {
		ERR_PRINT("Failed to init md5.");
		return FAILED;
	}

	return OK;
}

/*
 * Adds data to be hashed for md5.
 *
 * @param p_src - Pointer to data to be hashed.
 *
 * @param p_len - Length of the data in bytes.
 *
 * @return - OK, on success.
 *           FAILED, if input is null or update fails.
 */
Error CryptoCore::MD5Context::update(const uint8_t *p_src, size_t p_len) {
	if (p_len == 0) {
		// mbedtls accepts no-ops. No update needed.
		return OK;
	}

	if (!p_src) {
		ERR_PRINT("Null pointer passed to MD5 update.");
		return FAILED;
	}

	int ret = mbedtls_md5_update_ret(&ctx, p_src, p_len);

	if (ret != 0) {
		ERR_PRINT("Failed to create MD5 digest.");
		return FAILED;
	}

	return OK;
}

/*
 * Finalizes md5 hash operation, outputs digest.
 *
 * @param r_hash - Output buffer to store 16-byte MD5 hash.
 *
 * @return - OK, on success.
 *           FAILED, if initialization fails.
 */
Error CryptoCore::MD5Context::finish(unsigned char r_hash[16]) {
	int ret = mbedtls_md5_finish_ret(&ctx, r_hash);

	if (ret != 0) {
		ERR_PRINT("Failed to finalize, cannot output md5 digest.");
		return FAILED;
	}

	return OK;
}

// SHA1

/*
 * Constructor for SHA1 hash context.
 */
CryptoCore::SHA1Context::SHA1Context() {
	mbedtls_sha1_init(&ctx);
}

/*
 * Destructor for SHA1 hash context.
 */
CryptoCore::SHA1Context::~SHA1Context() {
	mbedtls_sha1_free(&ctx);
}

/*
 * Begins SHA1 hash operation.
 *
 * @return - OK, on success.
 *           FAILED, if initialization fails.
 */
Error CryptoCore::SHA1Context::start() {
	int ret = mbedtls_sha1_starts_ret(&ctx);

	if (ret != 0) {
		ERR_PRINT("Failed to start SHA1 hashing.");
		return FAILED;
	}

	return OK;
}

/*
 * Adds given data to hashed for SHA1.
 *
 * @param p_src - Pointer to the data to hash.
 *
 * @param p_len - Length of the data in bytes.
 *
 * @return - OK, on success.
 *           FAILED, if input is null or update fails.
 */
Error CryptoCore::SHA1Context::update(const uint8_t *p_src, size_t p_len) {
	if (p_len == 0) {
		// mbedtls accepts no-ops. No update needed.
		return OK;
	}

	if (!p_src) {
		ERR_PRINT("Null pointer passed to SHA1 update.");
		return FAILED;
	}

	int ret = mbedtls_sha1_update_ret(&ctx, p_src, p_len);

	if (ret != 0) {
		ERR_PRINT("Failed to create SHA1 digest.");
		return FAILED;
	}

	return OK;
}

/*
 * Finalizes sha1 operation, outputs digest.
 *
 * @param r_hash - Output buffer to store 20-byte SHA1 hash.
 *
 * @return - OK, if successful.
 *           FAILED, if finalization fails.
 */
Error CryptoCore::SHA1Context::finish(unsigned char r_hash[20]) {
	int ret = mbedtls_sha1_finish_ret(&ctx, r_hash);

	if (ret != 0) {
		ERR_PRINT("Failed to finalize SHA1 digest.");
		return FAILED;
	}

	return OK;
}

// SHA256

/*
 * Constructor for SHA256.
 */
CryptoCore::SHA256Context::SHA256Context() {
	mbedtls_sha256_init(&ctx);
}

/*
 * Destructor for SHA256.
 */
CryptoCore::SHA256Context::~SHA256Context() {
	mbedtls_sha256_free(&ctx);
}

/*
 * Begins SHA256 hash.
 *
 * @return - OK, if successful.
 *           FAILED, if not.
 */
Error CryptoCore::SHA256Context::start() {
	int ret = mbedtls_sha256_starts_ret(&ctx, 0); // is224 = 0

	if (ret != 0) {
		ERR_PRINT("Failed to start SHA256 hashing.");
		return FAILED;
	}

	return OK;
}

/*
 * Adds given data to hash.
 *
 * @param p_src - Pointer to data to be hashed.
 *
 * @param p_len - Length of data in bytes.
 *
 * @return - OK, if successful.
 *           FAILED, if input is null or update fails.
 */
Error CryptoCore::SHA256Context::update(const uint8_t *p_src, size_t p_len) {
	if (p_len == 0) {
		// mbedtls accepts no-ops. No update needed.
		return OK;
	}

	if (!p_src) {
		ERR_PRINT("Null pointer passed to SHA256 update.");
		return FAILED;
	}

	int ret = mbedtls_sha256_update_ret(&ctx, p_src, p_len);

	if (ret != 0) {
		ERR_PRINT("Failed to create SHA256 digest.");
		return FAILED;
	}

	return OK;
}

/*
 * Finalizes SHA256 hash, outputs digest.
 *
 * @param r_hash - output buffer to store 32-byte SHA256 hash.
 *
 * @return - OK, if successful.
 *           FAILED, if finalization fails.
 */
Error CryptoCore::SHA256Context::finish(unsigned char r_hash[32]) {
	int ret = mbedtls_sha256_finish_ret(&ctx, r_hash);

	if (ret != 0) {
		ERR_PRINT("Failed to finalize SHA256 hash.");
		return FAILED;
	}

	return OK;
}

// AES256

/*
 * Constructor for AES Context.
 */
CryptoCore::AESContext::AESContext() {
	mbedtls_aes_init(&ctx);
}

/*
 * Destructor for AES Context.
 */
CryptoCore::AESContext::~AESContext() {
	mbedtls_aes_free(&ctx);
}

/*
 * Sets AES encryption key with given key and key size.
 *
 * @param p_key - Pointer to encryption key.
 *
 * @param p_bits - Length of the key in bits, must be 128, 192, or 256.
 *
 * @return - OK, if successful.
 *           FAILED, if key invalid or initialization fails.
 */
Error CryptoCore::AESContext::set_encode_key(const uint8_t *p_key, size_t p_bits) {
	if (!p_key) {
		ERR_PRINT("No encryption key given.");
		return FAILED;
	}

	if (p_bits != 128 && p_bits != 192 && p_bits != 256) {
		ERR_PRINT("Invalid AES key size.");
		return FAILED;
	}

	int ret = mbedtls_aes_setkey_enc(&ctx, p_key, p_bits);

	if (ret != 0) {
		ERR_PRINT("Failed to set AES encryption key.");
		return FAILED;
	}

	return OK;
}

/*
 * Sets AES decryption key with given key and key size.
 *
 * @param p_key - Pointer to decryption key.
 *
 * @param p_bits - Length of key in bits, must be 128, 192, or 256.
 *
 * @return - OK, if successful.
 *           FAILED, if key invalid or initialization fails.
 */
Error CryptoCore::AESContext::set_decode_key(const uint8_t *p_key, size_t p_bits) {
	if (!p_key) {
		ERR_PRINT("No encryption key given.");
		return FAILED;
	}

	if (p_bits != 128 && p_bits != 192 && p_bits != 256) {
		ERR_PRINT("Invalid AES key size.");
		return FAILED;
	}

	int ret = mbedtls_aes_setkey_dec(&ctx, p_key, p_bits);

	if (ret != 0) {
		ERR_PRINT("Failed to set AES decryption key.");
		return FAILED;
	}

	return OK;
}

/*
 * Encrypts a single 16-byte plaintext using AES-ECB mode.
 *
 * @param p_src - Input buffer containing 16-byte plaintext.
 *
 * @param r_dst - Output buffer to store 16-byte ciphertext.
 *
 * @return - OK, if successful.
 *           FAILED, if encryption fails.
 */
Error CryptoCore::AESContext::encrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]) {
	int ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_ENCRYPT, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-ECB encryption.");
		return FAILED;
	}

	return OK;
}

/*
 * Encrypts plaintext using AES-CBC mode.
 *
 * @param p_length - Length of data in bytes, must be multiple of 16.
 *
 * @param r_iv - Input/Output buffer containing 16-byte IV, updates after use.
 *
 * @param p_src - Pointer to plaintext input buffer.
 *
 * @param r_dst - Pointer to ciphertext output buffer.
 *
 * @return - OK, if successful.
 *           FAILED, if encryption fails or inputs are invalid.
 */
Error CryptoCore::AESContext::encrypt_cbc(size_t p_length, uint8_t r_iv[16], const uint8_t *p_src, uint8_t *r_dst) {
	if (!p_src || !r_dst) {
		ERR_PRINT("Null pointer passed to encrypt AES-CBC");
		return FAILED;
	}

	int ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_ENCRYPT, p_length, r_iv, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-CBC encryption.");
		return FAILED;
	}

	return OK;
}

/*
 * Encrypt plaintext using AES-CFB128 mode.
 *
 * @p_length - Length of the data in bytes.
 *
 * @p_iv - Input/Output buffer containing 16-byte IV, updates after use.
 *
 * @p_src - Pointer to plaintext input buffer.
 *
 * @r_dst - Pointer to ciphertext output buffer.
 *
 * @return - OK, if successful.
 *           FAILED, if encryption fails or inputs are invalid.
 */
Error CryptoCore::AESContext::encrypt_cfb(size_t p_length, uint8_t p_iv[16], const uint8_t *p_src, uint8_t *r_dst) {
	if (!p_src || !r_dst) {
		ERR_PRINT("Null pointer passed to encrypt AES-CFB.");
		return FAILED;
	}

	size_t iv_off = 0; // Ignore and assume 16-byte alignment.
	int ret = mbedtls_aes_crypt_cfb128(&ctx, MBEDTLS_AES_ENCRYPT, p_length, &iv_off, p_iv, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-CFB encryption.");
		return FAILED;
	}

	return OK;
}

/*
 * Decrypts ciphertext using AES-ECB.
 *
 * @param p_src - Input buffer containing 16-byte ciphertext.
 *
 * @param r_dst - Output buffer to store 16-byte decrypted plaintext.
 *
 * @return - OK, on success.
 *           FAILED, if decryption fails or inputs are invalid.
 */
Error CryptoCore::AESContext::decrypt_ecb(const uint8_t p_src[16], uint8_t r_dst[16]) {
	if (!p_src || !r_dst) {
		ERR_PRINT("Null pointer passed to decrypt AES-ECB.");
		return FAILED;
	}
	int ret = mbedtls_aes_crypt_ecb(&ctx, MBEDTLS_AES_DECRYPT, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-ECB decryption.");
		return FAILED;
	}

	return OK;
}

/*
 * Decrypts ciphertext using AES-CBC.
 *
 * @param p_length - Length of the data in bytes, must be a multiple of 16.
 *
 * @param r_iv - Input/Output buffer containing 16-byte IV, updates after use.
 *
 * @param p_src - Pointer to ciphertext input buffer.
 *
 * @param r_dst - Pointer to plaintext output buffer.
 *
 * @return - OK, if successful.
 *           FAILED, if decryption fails or inputs are invalid.
 */
Error CryptoCore::AESContext::decrypt_cbc(size_t p_length, uint8_t r_iv[16], const uint8_t *p_src, uint8_t *r_dst) {
	if (!p_src || !r_dst) {
		ERR_PRINT("Null pointer passed to decrypt AES-CBC.");
		return FAILED;
	}

	int ret = mbedtls_aes_crypt_cbc(&ctx, MBEDTLS_AES_DECRYPT, p_length, r_iv, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-CBC decryption.");
		return FAILED;
	}

	return OK;
}

/*
 * Decrypts ciphertext using AES-CFB.
 *
 * @param p_length - Length of data in bytes.
 *
 * @param p_iv - Input/Output buffer containing 16-byte IV, updates after use.
 *
 * @param p_src - Pointer to ciphertext input buffer.
 *
 * @param r_dst - Pointer to plaintext output buffer.
 *
 * @return - OK, if successful.
 *           FAILED, if decryption fails or inputs are invalid.
 */
Error CryptoCore::AESContext::decrypt_cfb(size_t p_length, uint8_t p_iv[16], const uint8_t *p_src, uint8_t *r_dst) {
	if (!p_src || !r_dst) {
		ERR_PRINT("Null pointer passed to decrypt AES-CFB.");
		return FAILED;
	}

	size_t iv_off = 0; // Ignore and assume 16-byte alignment.
	int ret = mbedtls_aes_crypt_cfb128(&ctx, MBEDTLS_AES_DECRYPT, p_length, &iv_off, p_iv, p_src, r_dst);

	if (ret != 0) {
		ERR_PRINT("Failed to perform AES-CFB decryption.");
		return FAILED;
	}

	return OK;
}

// CryptoCore

/*
 * Encodes given string to base 64.
 *
 * @param p_src - Pointer to input.
 *
 * @param p_src_len - Length of input data in bytes.
 *
 * @return - Base64-encoded String, if successful.
 *           Empty String, on failure.
 */
String CryptoCore::b64_encode_str(const uint8_t *p_src, size_t p_src_len) {
	// Estimate max B64 length -- 4 output bytes for each 3 input bytes:
	const size_t b64len = ((p_src_len + 2) / 3) * 4 + 1; // + 1 for null terminator.

	Vector<uint8_t> b64buff;
	b64buff.resize(b64len);

	uint8_t *w64 = b64buff.ptrw();
	size_t strlen = 0;

	int ret = b64_encode(&w64[0], b64len, &strlen, p_src, p_src_len);

	if (strlen >= b64len) {
		ERR_PRINT("Base64 output exceeds buffer limit. Returning empty String.");
		return String();
	}

	if (ret != 0) {
		return String();
	}

	w64[strlen] = 0;

	return String((const char *)w64);
}

/*
 * Encodes given String to base64.
 *
 * @param r_dst - Point output buffer to store Base64-encoded data.
 *
 * @param p_dst_len - Size of output buffer in bytes.
 *
 * @param r_len - Pointer to store the number of bytes written to the output.
 */
Error CryptoCore::b64_encode(uint8_t *r_dst, size_t p_dst_len, size_t *r_len, const uint8_t *p_src, size_t p_src_len) {
	if (!r_dst || !r_len) {
		ERR_PRINT("Null pointer passed to base64 encode.");
		return FAILED;
	}

	int ret = mbedtls_base64_encode(r_dst, p_dst_len, r_len, p_src, p_src_len);

	if (ret != 0) {
		ERR_PRINT("Failed to encode to base64.");
		return FAILED;
	}

	return OK;
}

/*
 * Decodes given base64.
 *
 * @param r_dst - Pointer to output buffer for decoded data.
 *
 * @param p_dst_len - Length of the output buffer.
 *
 * @param r_len - Pointer to where length of decoded data will be stored.
 *
 * @param p_src - Pointer to input buffer containing base64-encoded data.
 *
 * @param p_src_len - Length of input buffer.
 *
 * @return - OK, if successful.
 *           FAILED, if buffer is null or decoding fails.
 */
Error CryptoCore::b64_decode(uint8_t *r_dst, size_t p_dst_len, size_t *r_len, const uint8_t *p_src, size_t p_src_len) {
	if (!r_dst || !r_len) {
		ERR_PRINT("Null pointer passed to base64 decode.");
		return FAILED;
	}

	int ret = mbedtls_base64_decode(r_dst, p_dst_len, r_len, p_src, p_src_len);

	if (ret != 0) {
		ERR_PRINT("Failed to decode from base64.");
		return FAILED;
	}

	return OK;
}

/*
 * Computes md5 digest with given data.
 *
 * @param p_src - Pointer to input data.
 *
 * @param p_src_len - Length of input data.
 *
 * @param r_hash - Output buffer to store 16-byte md5 hash.
 *
 * @return - OK, on success
 *           FAILED, if buffer is null or encoding fails.
 */
Error CryptoCore::md5(const uint8_t *p_src, size_t p_src_len, unsigned char r_hash[16]) {
	if (!p_src || !r_hash) {
		ERR_PRINT("Null pointer passed to md5.");
		return FAILED;
	}

	int ret = mbedtls_md5_ret(p_src, p_src_len, r_hash);

	if (ret != 0) {
		ERR_PRINT("Failed to compute MD5 hash.");
		return FAILED;
	}

	return OK;
}

/*
 * Computes sha1 digest with given data.
 *
 * @param p_src - Pointer to input data.
 *
 * @param p_src_len - Length of input data.
 *
 * @param r_hash - Output buffer to store 20-byte SHA1 hash.
 *
 * @return - OK, if successful
 *           FAILED, if buffer is null or encoding fails.
 */
Error CryptoCore::sha1(const uint8_t *p_src, size_t p_src_len, unsigned char r_hash[20]) {
	if (!p_src || !r_hash) {
		ERR_PRINT("Null pointer passed to sha1.");
		return FAILED;
	}

	int ret = mbedtls_sha1_ret(p_src, p_src_len, r_hash);

	if (ret != 0) {
		ERR_PRINT("Failed to compute SHA1 hash.");
		return FAILED;
	}

	return OK;
}

/*
 * Computes sha256 digest with given data.
 *
 * @param p_src - Pointer to the input data.
 *
 * @param p_src_len - Length of the input data.
 *
 * @param r_hash - Output buffer to store 32-byte SHA256 hash.
 *
 * @return - OK, on success.
 *           FAILED, if buffer is null or encoding fails.
 */
Error CryptoCore::sha256(const uint8_t *p_src, size_t p_src_len, unsigned char r_hash[32]) {
	if (!p_src || !r_hash) {
		ERR_PRINT("Null pointer passed to sha256.");
		return FAILED;
	}

	int ret = mbedtls_sha256_ret(p_src, p_src_len, r_hash, 0);

	if (ret != 0) {
		ERR_PRINT("Failed to compute SHA256 hash.");
		return FAILED;
	}

	return OK;
}
