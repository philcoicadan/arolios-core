/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <iostream>
#include <omm/Collection_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>

using namespace arolios;
using namespace arolios::omm;

void Collection_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Collection_type::has_properties () const {
	return false;
}

bool Collection_type::has_generalizations () const {
	return false;
}

std::shared_ptr<Logical_struct> Collection_type::logical_struct() const {
    //TODO
    std::cout << "Not implemented" << std::endl;
    return nullptr;
}

void Collection_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Collection_type> (p_elem) ;
}


void Collection_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Collection_type> (p_elem) ;
}

void Collection_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Collection_type> (p_elem, p_new_name) ;

}

void Collection_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for collection type
}

void Collection_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    // Nothing to do for collection type
}
        
bool Collection_type::has_elements (bool p_active_only) const {
    return false;
}


std::shared_ptr<Named_element> Collection_type::find_element ( const std::string& p_name) const {
   return nullptr;
}        
    

const std::string& Collection_type::meta_type() const {
    static std::string mt = "collection";
    return mt;
}

void Collection_type::rename_element (const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) {
    // Nothing to do for collection type
}

void Collection_type::classifier(const std::shared_ptr<Classifier>& p_classifier) {
    classifier_ = p_classifier;
}

const std::shared_ptr<Classifier>& Collection_type::classifier() const {
    return classifier_;
}

void Collection_type::kind(const Collection_kind p_kind) {
    kind_ = p_kind;
}

Collection_kind Collection_type::kind() const {
    return kind_;
}

void Collection_type::low(const int p_low) {
    low_ = p_low;
}

int Collection_type::low() const {
    return low_;
}

void Collection_type::high(const int p_high) {
    high_ = p_high;
}

int Collection_type::high() const {
    return high_;
}