/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Primitive_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

void Primitive_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Primitive_type::has_properties () const {
	return false;
}

bool  Primitive_type::has_generalizations () const {
	return false;
}

void Primitive_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Primitive_type> (p_elem) ;
}


void Primitive_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Primitive_type> (p_elem) ;
}

void Primitive_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name )
{
    p_pk.rename<Primitive_type> (p_elem, p_new_name) ;

}

void Primitive_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
	// Nothing to do - primitive types have no elements
}

void Primitive_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
	// Nothing to do - primitive types have no elements
}

bool Primitive_type::has_elements (bool p_active_only) const {
    return false;
}

std::shared_ptr<Named_element> Primitive_type::find_element ( const std::string& p_name) const {
    return nullptr;
}

const std::string& Primitive_type::meta_type() const {
    static std::string mt = "primitive";
    return mt;
}

void Primitive_type::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
	// Nothing to do - primitive types have no elements
}

void Primitive_type::native_type (const util::Native_type p_type) {
    native_type_ = p_type;
}

const util::Native_type Primitive_type::native_type () const {
    return native_type_;
}
