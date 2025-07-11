/**************************************************************************/
/*  engine_singletons.cpp                                                 */
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

#include "engine.h"

/*
 * Retrieves a list of registered singletons, optionally excluding
 * singletons marked as editor-only if the tools are not enabled.
 *
 * @param p_singletons - Pointer to a List that will be populated with
 *                       available singletons.
 */
void Engine::get_singletons(List<Singleton> *p_singletons) {
	// Clear list to prevent duplicate accumulation.
	p_singletons->clear();

	for (const Singleton &E : singletons) {
#ifdef TOOLS_ENABLED
		if (!is_editor_hint() && E.editor_only) {
			continue;
		}
#endif

		p_singletons->push_back(E);
	}
}

/*
 * Engine::Singleton constructor
 *
 * @para p_name - The name of the singleton.
 *
 * @para p_ptr - The pointer of the associated Object instance.
 *
 * @para p_class_name - The class name of the singleton.
 */
Engine::Singleton::Singleton(const StringName &p_name, Object *p_ptr, const StringName &p_class_name) :
		name(p_name),
		ptr(p_ptr),
		class_name(p_class_name) {
#ifdef DEBUG_ENABLED
	RefCounted *rc = Object::cast_to<RefCounted>(p_ptr);
	if (rc && !rc->is_referenced()) {
		WARN_PRINT("You must use Ref<> to ensure the lifetime of a RefCounted object intended to be used as a singleton.");
	}
#endif
}

/*
 * Removes a singleton from the engine's internal list.
 *
 * @param p_name - The name of the singleton to remove.
 */
void Engine::remove_singleton(const StringName &p_name) {
	ERR_FAIL_COND(!singleton_ptrs.has(p_name));

	for (List<Singleton>::Element *E = singletons.front(); E; E = E->next()) {
		if (E->get().name == p_name) {
			singletons.erase(E);
			singleton_ptrs.erase(p_name);
			return;
		}
	}
}

/*
 * Checks if a singleton is intended for use only in the editor.
 *
 * @para p_name - The name of the singleton to check.
 *
 * @return - True, if singleton is marked as editor-only.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_singleton_editor_only(const StringName &p_name) const {
	ERR_FAIL_COND_V(!singleton_ptrs.has(p_name), false);

	for (const Singleton &E : singletons) {
		if (E.name == p_name && E.editor_only) {
			return true;
		}
	}

	return false;
}

/*
 * Checks if a singleton with the given name was created by the user.
 *
 * @param p_name - The name of the singleton.
 *
 * @return - True, if singleton was user-created.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::is_singleton_user_created(const StringName &p_name) const {
	ERR_FAIL_COND_V(!singleton_ptrs.has(p_name), false);

	for (const Singleton &E : singletons) {
		if (E.name == p_name && E.user_created) {
			return true;
		}
	}

	return false;
}

/*
 * Adds a new singleton to the engine's internal list.
 *
 * @param - p_singleton - The Singleton to register.
 */
void Engine::add_singleton(const Singleton &p_singleton) {
	ERR_FAIL_COND_MSG(singleton_ptrs.has(p_singleton.name), vformat("Can't register singleton '%s' because it already exists.", p_singleton.name));
	ERR_FAIL_COND_MSG(p_singleton.ptr == nullptr, vformat("Can't register singleton '%s' with a nullptr.", p_singleton.name));
	singletons.push_back(p_singleton);
	singleton_ptrs[p_singleton.name] = p_singleton.ptr;
}

/*
 * Checks if a singleton with the given name exists within the engine.
 *
 * @param p_name - The name of the singleton to check.
 *
 * @return - True, if singleton exists.
 *           False, otherwise.
 */
[[nodiscard]] bool Engine::has_singleton(const StringName &p_name) const {
	return singleton_ptrs.has(p_name);
}

/*
 * Returns the global Engine singleton.
 */
Engine *Engine::get_singleton() {
	return singleton;
}

/*
 * Returns the singleton object with the given name.
 *
 * @param p_name - The name of the singleton to check.
 *
 * @return - Pointer to the singleton object, if found.
 *           nullptr, otherwise.
 */
Object *Engine::get_singleton_object(const StringName &p_name) const {
	HashMap<StringName, Object *>::ConstIterator E = singleton_ptrs.find(p_name);
	ERR_FAIL_COND_V_MSG(!E, nullptr, vformat("Failed to retrieve non-existent singleton '%s'.", p_name));

#ifdef TOOLS_ENABLED
	if (!is_editor_hint() && is_singleton_editor_only(p_name)) {
		ERR_FAIL_V_MSG(nullptr, vformat("Can't retrieve singleton '%s' outside of editor.", p_name));
	}
#endif

	return E->value;
}
