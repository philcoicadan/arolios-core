/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Composite_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

void Composite_type::accept (Visitor& p_visitor) const {
    p_visitor.visit(*this);
}

const std::string& Composite_type::meta_type() const {
    static std::string mt = "type";
    return mt;
}

void Composite_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.insert<Composite_type> (p_elem) ;
}

void Composite_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Composite_type> (p_elem) ;
}

void Composite_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name )
{
    p_pk.rename<Composite_type> (p_elem, p_new_name) ;
}
