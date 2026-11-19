/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include "omm/Named_element.h"
#include <omm/Enumeration.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Logical_enum.h>
#include <omm/Enum_value.h>


using namespace arolios;
using namespace arolios::omm;

void Enumeration::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

bool Enumeration::has_properties () const {
		return false;
}

bool  Enumeration::has_generalizations () const {
	return false;
}

void Enumeration::logical_enum (const std::shared_ptr<Logical_enum>& p_enum) {
    logical_enum_ = p_enum;
}

const std::shared_ptr<Logical_enum>& Enumeration::logical_enum () const {
    return logical_enum_;
}
        
std::shared_ptr<Logical_struct> Enumeration::logical_struct() const {
    return logical_enum_;
}

void Enumeration::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem){
    p_pk.insert<Enumeration> (p_elem) ;
}


void Enumeration::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Enumeration> (p_elem) ;
}

void Enumeration::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<Enumeration> (p_elem, p_new_name) ;

}

void Enumeration::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto && elem = std::dynamic_pointer_cast<Enum_value>(p_elem);
    if (elem) {
        values_.insert(std::make_pair<const std::string&,const std::shared_ptr<Enum_value>&>(elem->name(),elem));
    }
}

void Enumeration::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
    auto search = values_.find(p_elem->name());
    if ( search != values_.end() ) {
        values_.erase (search->first) ;
    } else {
        throw exception::Failure("enumeration value " + p_elem->name() + " not found" );
    }
}

const std::unordered_set<std::string> Enumeration::values () const {
    std::unordered_set<std::string> result;
    for ( auto [value, obj]: values_) {
        result.insert(value);
    }
    return result;
}
        
//std::string Enumeration::select_template() { return ""; }

bool Enumeration::has_elements (bool p_active_only) const {

    if ( !p_active_only) {
        return values_.empty();
    } else {
        bool result = false;
        for (auto  it = values_.begin(); it != values_.end() && result == false ; ++it ) {
            if (it->second->lc_status() != Lc_status::active) continue;
            result = true;
        }
        return result;
    }
}

void Enumeration::recursive_lc_status(const omm::Lc_status p_status)
{

    for ( auto  it = values_.begin(); it != values_.end()  ; ++it ) {
        it->second->lc_status ( p_status );

    }

    lc_status ( p_status );
}


std::shared_ptr<Named_element> Enumeration::find_element( const std::string& p_name) const {
   auto search = values_.find(p_name) ;
   return  (search== values_.end()) ? nullptr : search->second;
}        
    
    
const std::string& Enumeration::meta_type() const {
    static std::string mt = "enumeration";
    return mt;
}

void Enumeration::rename_element (const std::shared_ptr<omm::Packageable_element>& p_elem, const std::string& p_new_name) {
    auto && elem = std::dynamic_pointer_cast<Enum_value>(p_elem);
    if (elem) {
        values_.insert(std::make_pair<const std::string&,const std::shared_ptr<Enum_value>&>(p_new_name,elem));
        values_.erase(p_elem->name());
    
    }
}