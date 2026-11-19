/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <exception/Exceptions.h>
#include <omm/Selection_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

void Selection_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Selection_type::has_properties () const {
	return false;
}

bool  Selection_type::has_generalizations () const {
	return false;
}

bool Selection_type::has_id () const {
    // TODO
    throw exception::Not_yet_implemented("Selection_type::has_id");
    return false;
}

std::shared_ptr<Logical_struct> Selection_type::logical_struct() const {
    if ( !options_.empty() ) {
        return options_[0] ? options_[0]->logical_struct() : nullptr;
    }
    return nullptr;
}

void Selection_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Selection_type> (p_elem) ;
}


void Selection_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Selection_type> (p_elem) ;
}

void Selection_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Selection_type> (p_elem, p_new_name) ;

}

void Selection_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for selection type
}

void Selection_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for selection type
}
        
bool Selection_type::has_elements (bool p_active_only) const {
    return false;
}


std::shared_ptr<Named_element> Selection_type::find_element ( const std::string& p_name) const {
   return nullptr;
}        
    

const std::string& Selection_type::meta_type() const {
    static std::string mt = "selection";
    return mt;
}

void Selection_type::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
    // Nothing to do for selection type
}

void Selection_type::options(const std::vector<std::shared_ptr<Classifier>>& p_classifiers) {
    options_ = p_classifiers;
}

const std::vector<std::shared_ptr<Classifier>>& Selection_type::options() const {
    return options_;
}
