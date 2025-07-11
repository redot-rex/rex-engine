/**************************************************************************/
/*  crypto_TLS.cpp                                                        */
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
