/**************************************************************************/
/*  hashing_context.cpp                                                   */
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

#include "hashing_context.h"

/*
 * Constructor for HashingContext.
 */
HashingContext::HashingContext() {}

/*
 * Destructor for HashingContext.
 */
HashingContext::~HashingContext() {}

/*
 * Initialize given hash type context.
 *
 * @param p_type - Type of hash algorithm to initialize (e.g. MD5, SHA1, SHA256)
 *
 * @return - OK, on success.
 *           ERR_UNVAILABLE, if unsupported hash type.
 */
Error HashingContext::start(HashType p_type) {
	// Set Hash type
	type = p_type;

	switch (type) {
		case HASH_MD5:
			return md5_ctx.start();
		case HASH_SHA1:
			return sha1_ctx.start();
		case HASH_SHA256:
			return sha256_ctx.start();
		default:
			ERR_PRINT("Unsupported hash type.");
			return ERR_UNAVAILABLE;
	}
}

/*
 * Add given data to hash.
 *
 * @param p_chunk - PackedByteArray containing data to be hashed.
 *
 * @return - OK, if empty/no-ops or successful.
 *           ERR_UNAVAILABLE, if hash type is invalid.
 */
Error HashingContext::update(const PackedByteArray &p_chunk) {
	if (p_chunk.is_empty()) {
		// mbedtls accepts no-ops. No update needed.
		return OK;
	}

	size_t len = p_chunk.size();
	const uint8_t *r = p_chunk.ptr();

	switch (type) {
		case HASH_MD5:
			return md5_ctx.update(r, len);
		case HASH_SHA1:
			return sha1_ctx.update(r, len);
		case HASH_SHA256:
			return sha256_ctx.update(r, len);
		default:
			ERR_PRINT("Invalid state in HashingContext::update().");
			return ERR_UNAVAILABLE;
	}
}

/*
 * Finalize hash op, returns digest.
 *
 * @return - PackedByteArray, if digest computed.
 *           Empty PackedByteArray, if invalid hash or failed.
 */
PackedByteArray HashingContext::finish() {
	PackedByteArray out;
	Error err = FAILED;

	switch (type) {
		case HASH_MD5:
			out.resize(16);
			err = md5_ctx.finish(out.ptrw());
			break;
		case HASH_SHA1:
			out.resize(20);
			err = sha1_ctx.finish(out.ptrw());
			break;
		case HASH_SHA256:
			out.resize(32);
			err = sha256_ctx.finish(out.ptrw());
			break;
		default:
			ERR_PRINT("Invalid state in HashingContext::finish().");
			return PackedByteArray();
	}

	if (err != OK) {
		ERR_PRINT("HashingContext::finish() failed.");
		return PackedByteArray();
	}

	return out;
}

/*
 * Binds both public functions plus enums.
 */
void HashingContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start", "type"), &HashingContext::start);
	ClassDB::bind_method(D_METHOD("update", "chunk"), &HashingContext::update);
	ClassDB::bind_method(D_METHOD("finish"), &HashingContext::finish);
	BIND_ENUM_CONSTANT(HASH_MD5);
	BIND_ENUM_CONSTANT(HASH_SHA1);
	BIND_ENUM_CONSTANT(HASH_SHA256);
}
