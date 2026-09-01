/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Restriction_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Constraint.h>

using namespace arolios;
using namespace arolios::omm;

void Restriction_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Restriction_type::has_properties () const {
     return false;
}

bool  Restriction_type::has_generalizations () const {
    return false;
}


void Restriction_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Restriction_type> (p_elem) ;
}


void Restriction_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Restriction_type> (p_elem) ;
}

void Restriction_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Restriction_type> (p_elem, p_new_name) ;

}

void Restriction_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
     auto && elem = std::dynamic_pointer_cast<Constraint>(p_elem);
    if (elem) {
        insert_constraint(elem);
    }

}

void Restriction_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
     auto && elem = std::dynamic_pointer_cast<Constraint>(p_elem);
    if (elem) {
        erase_constraint(elem);
    }
}
        
bool Restriction_type::has_elements (bool p_active_only) const {
    if ( !p_active_only) {
        return constraints().empty();
    } else {
        bool result = false;
        for (auto  it = constraints().begin(); it != constraints().end() && result == false ; ++it ) {
            if (it->second->lc_status() != Lc_status::active) continue;
            result = true;
        }
        return result;
    }
}


std::shared_ptr<Named_element> Restriction_type::find_element ( const std::string& p_name) const {
    return referenced_type_ ? referenced_type_->find_element(p_name) : nullptr;
}        
    

const std::string& Restriction_type::meta_type() const {
    static std::string mt = "restriction";
    return mt;
}

void Restriction_type::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
      //  if (referenced_type_ ) referenced_type_->rename_element (p_elem, p_new_name);
}

void Restriction_type::referenced_type(const std::shared_ptr<Data_type>& p_type) {
    referenced_type_ = p_type;
}

const std::shared_ptr<Data_type>& Restriction_type::referenced_type() const {
    return referenced_type_;
}

