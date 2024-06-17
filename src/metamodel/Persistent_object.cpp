/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Persistent_object.h>


using namespace arolios;
using namespace arolios::omm;

Persistent_object::Persistent_object() :
id_ (std::to_string( 0 )) , scope_(std::string()) {
}

Persistent_object::~Persistent_object() {

}

void Persistent_object::id (const std::string& p_id) { id_ = p_id; }
const std::string& Persistent_object::id () const { return id_; }
void Persistent_object::id_scope(const std::string& p_scope ) { scope_ = p_scope; }
const std::string& Persistent_object::id_scope () const { return scope_; }

void Persistent_object::persistent_attributes (const std::string& p_id, const std::string& p_scope)  {
    id_ = p_id;
    scope_ = p_scope;
}
