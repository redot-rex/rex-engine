/**************************************************************************/
/*  aes_context.cpp                                                       */
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

#include "core/crypto/aes_context.h"

// TODO: Integrate AES-GCM -- This provides integrity/authentication.
// AESContext is unusable for anything outside of session-local obfuscation.

/*
 * Helper function to check if AES is initialized.
 *
 * @return - True, if AES mode is within a valid range.
 *           False, otherwise.
 */
bool AESContext::is_initialized() const {
	return (mode >= 0 && mode < MODE_MAX);
}

/*
 * Initializes AES.
 *
 * @param p_mode - AES operation mode.
 *
 * @param p_key - Encryption key, must be 16bytes or 32bytes.
 *
 * @param p_iv - The Initialization Vector, must be 16bytes for AES-CBC mode.
 *
 * @return - OK, on success.
 *           Appropriate Error code, on failure.
 */
Error AESContext::start(Mode p_mode, const PackedByteArray &p_key, const PackedByteArray &p_iv) {
	// Ensures not already started
	if (mode != MODE_MAX) {
		ERR_PRINT("AESContext already started. Call 'finish' before starting a new one.");
		return ERR_ALREADY_IN_USE;
	}

	// Validates requested mode.
	if (p_mode < 0 || p_mode >= MODE_MAX) {
		ERR_PRINT("Invalid AES mode request.");
		return ERR_INVALID_PARAMETER;
	}

	// Convert from bytes to bits
	const int key_bits = p_key.size() << 3;

	// Verify key length
	if (key_bits != 128 && key_bits != 256) {
		ERR_PRINT("AES key must be either 16 or 32 bytes");
		return ERR_INVALID_PARAMETER;
	}

	// Init for CBC mode
	if (p_mode == MODE_CBC_ENCRYPT || p_mode == MODE_CBC_DECRYPT) {
		if (p_iv.size() != AES_BLOCK_SIZE) {
			ERR_PRINT("The initialization vector (IV) must be exactly 16 bytes.");
			return ERR_INVALID_PARAMETER;
		}
		iv = p_iv; // direct assignment makes more sense vs resizing and appending.
	}

	const uint8_t *key_data = p_key.ptr();

	// Warn about deprecation
	if (p_mode == MODE_ECB_ENCRYPT) {
		WARN_PRINT("AES-ECB mode is insecure and not recommended.");
	}

	// Initialize internal AES with correct keytype.
	if (p_mode == MODE_CBC_ENCRYPT || p_mode == MODE_ECB_ENCRYPT) {
		ctx.set_encode_key(key_data, key_bits);
	} else {
		ctx.set_decode_key(key_data, key_bits);
	}

	mode = p_mode;

	return OK;
}

/*
 * Processes AES encryption/decryption on given input.
 *
 * @param p_src - Input data to be encrypted/decrypted, must be non-zero
 *                multiples of 16 bytes.
 *
 * @return - The encrypted/decrypted output as a PackedByteArray.
 *           An empty PackedByteArray, on failure.
 */
PackedByteArray AESContext::update(const PackedByteArray &p_src) {
	// TODO: Add PKCS#7 padding so users don't need to provide padding.

	// Verify AES has been initialized.
	if (!is_initialized()) {
		ERR_PRINT("AESContext not started. Call 'start' before calling 'update'.");
		return PackedByteArray();
	}

	const int len = p_src.size();

	if (len % AES_BLOCK_SIZE != 0 || len == 0) {
		ERR_PRINT("The number of bytes to be encrypted must be multiple of 16. Add padding if needed.");
		return PackedByteArray();
	}

	PackedByteArray out;
	out.resize(len);

	// Write access need because CBC mode expects to mutate IV
	// FIXME: Enforce refreshing IVs for CBC to prevent IV chaining.
	uint8_t *p_iv = iv.ptrw();
	// Read only input buffer
	const uint8_t *src_ptr = p_src.ptr();
	// Writable output buffer
	uint8_t *out_ptr = out.ptrw();
	// 16-byte blocks
	const int block_count = len / AES_BLOCK_SIZE;
	// Using `volatile` to prevent compiler optimizations (e.g skip or
	// reorder writes) and could interfere with constant-time execution and
	// could leak timing side channels. NOTE: This only helps with
	// compilers, but not speculative execution / branch prediction /
	// CPU-level leaks (e.g. Spectre/Meltdown)
	volatile int result = OK;

	switch (mode) {
		case MODE_ECB_ENCRYPT: {
			WARN_PRINT("AES-ECB mode is insecure and not recommended.");
			// Processing each 16-byte block for AES-ECB encryption.
			for (int i = 0; i < block_count; ++i) {
				result |= (ctx.encrypt_ecb(src_ptr + (i * AES_BLOCK_SIZE), out_ptr + (i * AES_BLOCK_SIZE)) != OK);
			}
			if (result != OK) {
				ERR_PRINT("AES-ECB encrypt block(s) failed.");
				return PackedByteArray();
			}
		} break;
		case MODE_ECB_DECRYPT: {
			WARN_PRINT("AES-ECB mode is insecure and not recommended.");
			// Processing each 16-byte block for AES-ECB decryption.
			for (int i = 0; i < block_count; ++i) {
				result |= (ctx.decrypt_ecb(src_ptr + (i * AES_BLOCK_SIZE), out_ptr + (i * AES_BLOCK_SIZE)) != OK);
			}
			if (result != OK) {
				ERR_PRINT("AES-ECB decrypt block(s) failed.");
				return PackedByteArray();
			}
		} break;
		case MODE_CBC_ENCRYPT: {
			// Process all blocks for AES-CBC encryption.
			Error err = ctx.encrypt_cbc(len, p_iv, src_ptr, out_ptr);
			if (err != OK) {
				ERR_PRINT("AES-CBC encrypt failed.");
				return PackedByteArray();
			}
		} break;
		case MODE_CBC_DECRYPT: {
			// Process all blocks for AES-CBC decryption.
			Error err = ctx.decrypt_cbc(len, p_iv, src_ptr, out_ptr);
			if (err != OK) {
				ERR_PRINT("AES-CBC decrypt failed.");
				return PackedByteArray();
			}
		} break;
		default:
			ERR_PRINT("Invalid AES mode. How did you get here? Contact a dev.");
			return PackedByteArray();
	}

	return out;
}

/*
 * Returns current IV state.
 *
 * @return - The current IV as a PackedByteArray.
 *           An empty PackedByteArray, if not in CBC mode.
 */
PackedByteArray AESContext::get_iv_state() {
	if (mode != MODE_CBC_ENCRYPT && mode != MODE_CBC_DECRYPT) {
		ERR_PRINT("Calling 'get_iv_state' only makes sense when the context is started in CBC mode.");
		return PackedByteArray();
	}

	return iv;
}

/*
 * Finalize AES, clear internal state.
 */
void AESContext::finish() {
	// Securely wipes the IV.
	std::fill(iv.ptrw(), iv.ptrw() + iv.size(), 0);
	// Mark as inactive.
	mode = MODE_MAX;
	// Clear IV buffer.
	iv.clear();
	// Clear the context by switching to default.
	ctx = CryptoCore::AESContext();
}

/*
 * Bind AESContext for scripting access.
 */
void AESContext::_bind_methods() {
	// Bind functions
	ClassDB::bind_method(D_METHOD("start", "mode", "key", "iv"), &AESContext::start, DEFVAL(PackedByteArray()));
	ClassDB::bind_method(D_METHOD("update", "src"), &AESContext::update);
	ClassDB::bind_method(D_METHOD("get_iv_state"), &AESContext::get_iv_state);
	ClassDB::bind_method(D_METHOD("finish"), &AESContext::finish);

	// Bind AES modes as constants.
	BIND_ENUM_CONSTANT(MODE_ECB_ENCRYPT);
	BIND_ENUM_CONSTANT(MODE_ECB_DECRYPT);
	BIND_ENUM_CONSTANT(MODE_CBC_ENCRYPT);
	BIND_ENUM_CONSTANT(MODE_CBC_DECRYPT);
	BIND_ENUM_CONSTANT(MODE_MAX);
}

/*
 * Constructor
 */
AESContext::AESContext() {}

/*
 * Destructor
 */
AESContext::~AESContext() {
	finish(); // Just in case it was forgotten.
}
