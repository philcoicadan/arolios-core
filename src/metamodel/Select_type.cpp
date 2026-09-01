/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Select_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

void Select_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Select_type::has_properties () const {
	return false;
}

bool  Select_type::has_generalizations () const {
	return false;
}

std::shared_ptr<Logical_struct> Select_type::logical_struct() const {
    if ( !classifiers_.empty() ) {
        return classifiers_[0] ? classifiers_[0]->logical_struct() : nullptr;
    }
    return nullptr;
}

void Select_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Select_type> (p_elem) ;
}


void Select_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Select_type> (p_elem) ;
}

void Select_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Select_type> (p_elem, p_new_name) ;

}

void Select_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for select type
}

void Select_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for select type
}
        
bool Select_type::has_elements (bool p_active_only) const {
    return false;
}


std::shared_ptr<Named_element> Select_type::find_element ( const std::string& p_name) const {
   return nullptr;
}        
    

const std::string& Select_type::meta_type() const {
    static std::string mt = "select";
    return mt;
}

void Select_type::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
    // Nothing to do for select type
}

void Select_type::classifiers(const std::vector<std::shared_ptr<Classifier>>& p_classifiers) {
    classifiers_ = p_classifiers;
}

const std::vector<std::shared_ptr<Classifier>>& Select_type::classifiers() const {
    return classifiers_;
}
