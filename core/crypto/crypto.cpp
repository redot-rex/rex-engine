/**************************************************************************/
/*  crypto.cpp                                                            */
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

#include "crypto.h"

/// Resources

/*
 * Creates a new CryptoKey instance.
 *
 * @param p_notify_postinitialize - Whether or not to notify after post-init.
 *
 * @return - Pointer to CryptoKey, if successful.
 *           nullptr, if unsuccessful.
 */
CryptoKey *(*CryptoKey::_create)(bool p_notify_postinitialize) = nullptr;
CryptoKey *CryptoKey::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	return nullptr;
}

void CryptoKey::_bind_methods() {
	ClassDB::bind_method(D_METHOD("save", "path", "public_only"), &CryptoKey::save, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("load", "path", "public_only"), &CryptoKey::load, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("is_public_only"), &CryptoKey::is_public_only);
	ClassDB::bind_method(D_METHOD("save_to_string", "public_only"), &CryptoKey::save_to_string, DEFVAL(false));
	ClassDB::bind_method(D_METHOD("load_from_string", "string_key", "public_only"), &CryptoKey::load_from_string, DEFVAL(false));
}

/*
 * Creates a new X590Certificate instance.
 *
 * @param p_notify_postinitialize - Whether or not to notify after post-init.
 *
 * @return - Pointer to new X509Certificate, if successful.
 *           nullptr, if not successful.
 */
X509Certificate *(*X509Certificate::_create)(bool p_notify_postinitialize) = nullptr;
X509Certificate *X509Certificate::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	return nullptr;
}

void X509Certificate::_bind_methods() {
	ClassDB::bind_method(D_METHOD("save", "path"), &X509Certificate::save);
	ClassDB::bind_method(D_METHOD("load", "path"), &X509Certificate::load);
	ClassDB::bind_method(D_METHOD("save_to_string"), &X509Certificate::save_to_string);
	ClassDB::bind_method(D_METHOD("load_from_string", "string"), &X509Certificate::load_from_string);
}

/// TLSOptions

/*
 * Create and configure TLSOptions for client mode.
 *
 * @param p_trusted_chain - The X509 cert chain.
 *
 * @param p_common_name_override - Common name override for cert verification (optional.)
 *
 * @return - A configured Ref<TLSOptions> instance, in client mode.
 */
Ref<TLSOptions> TLSOptions::client(Ref<X509Certificate> p_trusted_chain, const String &p_common_name_override) {
	Ref<TLSOptions> opts;
	opts.instantiate();
	opts->mode = MODE_CLIENT;
	opts->trusted_ca_chain = p_trusted_chain;
	opts->common_name = p_common_name_override;
	return opts;
}

/*
 * Creates TLSOptions instance for client mode, without strict cert
 * verification allowing for optional CA verification.
 *
 * @param p_trusted_chain - The X509 cert chain.
 *
 * @return - A configured Ref<TLSOptions> instance, in unsafe client mode.
 */
Ref<TLSOptions> TLSOptions::client_unsafe(Ref<X509Certificate> p_trusted_chain) {
	Ref<TLSOptions> opts;
	opts.instantiate();
	opts->mode = MODE_CLIENT_UNSAFE;
	opts->trusted_ca_chain = p_trusted_chain;
	return opts;
}

/*
 * Create TLSOptions instance, in server mode.
 *
 * @param p_own_key - Server's private CryptoKey.
 *
 * @param p_own_certificate - Server's x509 cert.
 *
 * @return - A configured Ref<TLSOptions> instance, in server mode.
 */
Ref<TLSOptions> TLSOptions::server(Ref<CryptoKey> p_own_key, Ref<X509Certificate> p_own_certificate) {
	Ref<TLSOptions> opts;
	opts.instantiate();
	opts->mode = MODE_SERVER;
	opts->own_certificate = p_own_certificate;
	opts->private_key = p_own_key;
	return opts;
}

void TLSOptions::_bind_methods() {
	ClassDB::bind_static_method("TLSOptions", D_METHOD("client", "trusted_chain", "common_name_override"), &TLSOptions::client, DEFVAL(Ref<X509Certificate>()), DEFVAL(String()));
	ClassDB::bind_static_method("TLSOptions", D_METHOD("client_unsafe", "trusted_chain"), &TLSOptions::client_unsafe, DEFVAL(Ref<X509Certificate>()));
	ClassDB::bind_static_method("TLSOptions", D_METHOD("server", "key", "certificate"), &TLSOptions::server);

	ClassDB::bind_method(D_METHOD("is_server"), &TLSOptions::is_server);
	ClassDB::bind_method(D_METHOD("is_unsafe_client"), &TLSOptions::is_unsafe_client);
	ClassDB::bind_method(D_METHOD("get_common_name_override"), &TLSOptions::get_common_name_override);
	ClassDB::bind_method(D_METHOD("get_trusted_ca_chain"), &TLSOptions::get_trusted_ca_chain);
	ClassDB::bind_method(D_METHOD("get_private_key"), &TLSOptions::get_private_key);
	ClassDB::bind_method(D_METHOD("get_own_certificate"), &TLSOptions::get_own_certificate);
}

/// HMACContext

void HMACContext::_bind_methods() {
	ClassDB::bind_method(D_METHOD("start", "hash_type", "key"), &HMACContext::start);
	ClassDB::bind_method(D_METHOD("update", "data"), &HMACContext::update);
	ClassDB::bind_method(D_METHOD("finish"), &HMACContext::finish);
}

/*
 * Creates a new HMACContext instance.
 *
 * @param p_notify_postinitialize - Whether or not to notify after post-init.
 *
 * @return - A pointer to new HMACContext instance, if successful.
 *           nullptr, if not successful.
 */
HMACContext *(*HMACContext::_create)(bool p_notify_postinitialize) = nullptr;
HMACContext *HMACContext::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	ERR_FAIL_V_MSG(nullptr, "HMACContext is not available when the mbedtls module is disabled.");
}

/// Crypto

/*
 * Load default cert from specified path.
 *
 * @param p_path - Path to directory or file containing default cert.
 */
void (*Crypto::_load_default_certificates)(const String &p_path) = nullptr;

/*
 * Creates new Crypto instance.
 *
 * @param p_notify_postinitialize - Whether or not to notify after post-init.
 *
 * @return - A pointer to new Crypto instance, if successful.
 *           nullptr, if not successful.
 */
Crypto *(*Crypto::_create)(bool p_notify_postinitialize) = nullptr;
Crypto *Crypto::create(bool p_notify_postinitialize) {
	if (_create) {
		return _create(p_notify_postinitialize);
	}
	ERR_FAIL_V_MSG(nullptr, "Crypto is not available when the mbedtls module is disabled.");
}

/*
 * Load default cert from specified path.
 *
 * @param p_path - The path to the default cert store.
 */
void Crypto::load_default_certificates(const String &p_path) {
	if (_load_default_certificates) {
		_load_default_certificates(p_path);
	}
}

/*
 * Computes HMAC digest using given hash type, key, and data.
 *
 * @param p_hash_type - The hash algorithm to use.
 *
 * @param p_key - The secret key for HMAC.
 *
 * @param p_msg - The message to authenticate.
 *
 * @return - HMAC digest as PackedByteArray, if successful.
 *           empty PackedByteArray, if failed.
 */
PackedByteArray Crypto::hmac_digest(HashingContext::HashType p_hash_type, const PackedByteArray &p_key, const PackedByteArray &p_msg) {
	Ref<HMACContext> ctx = Ref<HMACContext>(HMACContext::create());
	ERR_FAIL_COND_V_MSG(ctx.is_null(), PackedByteArray(), "HMAC is not available without mbedtls module.");
	Error err = ctx->start(p_hash_type, p_key);
	ERR_FAIL_COND_V(err != OK, PackedByteArray());
	err = ctx->update(p_msg);
	ERR_FAIL_COND_V(err != OK, PackedByteArray());
	return ctx->finish();
}

// Compares two HMACS for equality without leaking timing information in order to prevent timing attacks.
// @see: https://paragonie.com/blog/2015/11/preventing-timing-attacks-on-string-comparison-with-double-hmac-strategy
bool Crypto::constant_time_compare(const PackedByteArray &p_trusted, const PackedByteArray &p_received) {
	const uint8_t *t = p_trusted.ptr();
	const uint8_t *r = p_received.ptr();
	int tlen = p_trusted.size();
	int rlen = p_received.size();
	// If the lengths are different then nothing else matters.
	if (tlen != rlen) {
		return false;
	}

	uint8_t v = 0;
	for (int i = 0; i < tlen; i++) {
		v |= t[i] ^ r[i];
	}
	return v == 0;
}

void Crypto::_bind_methods() {
	ClassDB::bind_method(D_METHOD("generate_random_bytes", "size"), &Crypto::generate_random_bytes);
	ClassDB::bind_method(D_METHOD("generate_rsa", "size"), &Crypto::generate_rsa);
	ClassDB::bind_method(D_METHOD("generate_self_signed_certificate", "key", "issuer_name", "not_before", "not_after"), &Crypto::generate_self_signed_certificate, DEFVAL("CN=myserver,O=myorganisation,C=IT"), DEFVAL("20140101000000"), DEFVAL("20340101000000"));
	ClassDB::bind_method(D_METHOD("sign", "hash_type", "hash", "key"), &Crypto::sign);
	ClassDB::bind_method(D_METHOD("verify", "hash_type", "hash", "signature", "key"), &Crypto::verify);
	ClassDB::bind_method(D_METHOD("encrypt", "key", "plaintext"), &Crypto::encrypt);
	ClassDB::bind_method(D_METHOD("decrypt", "key", "ciphertext"), &Crypto::decrypt);
	ClassDB::bind_method(D_METHOD("hmac_digest", "hash_type", "key", "msg"), &Crypto::hmac_digest);
	ClassDB::bind_method(D_METHOD("constant_time_compare", "trusted", "received"), &Crypto::constant_time_compare);
}

/// Resource loader/saver

/*
 * Load a cryptographic resource.
 *
 * @param p_path - The path to the resource file.
 * 
 * @param p_original_path - The original path before remapped (unused.)
 *
 * @param r_error - Pointer to store resulting err code (optional.)
 *
 * @param p_use_sub_threads - (unused.)
 * 
 * @param r_progress - (unused pointer.)
 *
 * @param p_cache_mode - (unused.)
 *
 * @return - Ref<Resource> to the loaded cryptographic obj, if successful.
 *           nullptr, if failed.
 */
Ref<Resource> ResourceFormatLoaderCrypto::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	String el = p_path.get_extension().to_lower();
	if (el == "crt") {
		X509Certificate *cert = X509Certificate::create();
		if (cert) {
			cert->load(p_path);
		}
		return cert;
	} else if (el == "key") {
		CryptoKey *key = CryptoKey::create();
		if (key) {
			key->load(p_path, false);
		}
		return key;
	} else if (el == "pub") {
		CryptoKey *key = CryptoKey::create();
		if (key) {
			key->load(p_path, true);
		}
		return key;
	}
	return nullptr;
}

/*
 * Return list of file extensions recognized by this loader.
 *
 * @param p_extensions - Pointer to list to populate with supported extensions.
 */
void ResourceFormatLoaderCrypto::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("crt");
	p_extensions->push_back("key");
	p_extensions->push_back("pub");
}

/*
 * Check if loader can handle given resource type.
 *
 * @param p_type - Resource type to check.
 *
 * @return - True, if supported.
 *           False, otherwise.
 */
bool ResourceFormatLoaderCrypto::handles_type(const String &p_type) const {
	return p_type == "X509Certificate" || p_type == "CryptoKey";
}

/*
 * Determine resource type based on file extension.
 *
 * @param p_path - file path to eval.
 *
 * @return - Resource type as String, if successful.
 *           Empty String, if unknown.
 */
String ResourceFormatLoaderCrypto::get_resource_type(const String &p_path) const {
	String el = p_path.get_extension().to_lower();
	if (el == "crt") {
		return "X509Certificate";
	} else if (el == "key" || el == "pub") {
		return "CryptoKey";
	}
	return "";
}

/*
 * Save cryptographic resource to given file path.
 *
 * @param p_resource - The cryptographic resource to save.
 *
 * @param p_path - The target file path.
 *
 * @param p_flags - (Unused.)
 *
 * @return - OK, on success.
 *           Appropriate Error code, on failure.
 */
Error ResourceFormatSaverCrypto::save(const Ref<Resource> &p_resource, const String &p_path, uint32_t p_flags) {
	Error err;
	Ref<X509Certificate> cert = p_resource;
	Ref<CryptoKey> key = p_resource;
	if (cert.is_valid()) {
		err = cert->save(p_path);
	} else if (key.is_valid()) {
		String el = p_path.get_extension().to_lower();
		err = key->save(p_path, el == "pub");
	} else {
		ERR_FAIL_V(ERR_INVALID_PARAMETER);
	}
	ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Cannot save Crypto resource to file '%s'.", p_path));
	return OK;
}

/*
 * Return list of file extensions applicable for given crypto resource.
 *
 * @param p_resource - The resource to eval.
 *
 * @param p_extensions - Pointer to list to populate with valid extensions.
 */
void ResourceFormatSaverCrypto::get_recognized_extensions(const Ref<Resource> &p_resource, List<String> *p_extensions) const {
	const X509Certificate *cert = Object::cast_to<X509Certificate>(*p_resource);
	const CryptoKey *key = Object::cast_to<CryptoKey>(*p_resource);
	if (cert) {
		p_extensions->push_back("crt");
	}
	if (key) {
		if (!key->is_public_only()) {
			p_extensions->push_back("key");
		}
		p_extensions->push_back("pub");
	}
}

/*
 * Check if given resource is supported.
 *
 * @param p_resource - Resource to check.
 *
 * @return - True, if supported.
 *           False, otherwise.
 */
bool ResourceFormatSaverCrypto::recognize(const Ref<Resource> &p_resource) const {
	return Object::cast_to<X509Certificate>(*p_resource) || Object::cast_to<CryptoKey>(*p_resource);
}
