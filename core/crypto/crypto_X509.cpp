/**************************************************************************/
/*  crypto_X509.cpp                                                       */
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

void X509Certificate::_bind_methods() {
    ClassDB::bind_method(D_METHOD("save", "path"), &X509Certificate::save);
    ClassDB::bind_method(D_METHOD("load", "path"), &X509Certificate::load);
    ClassDB::bind_method(D_METHOD("save_to_string"), &X509Certificate::save_to_string);
    ClassDB::bind_method(D_METHOD("load_from_string", "string"), &X509Certificate::load_from_string);
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


