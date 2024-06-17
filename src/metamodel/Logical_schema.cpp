/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Logical_schema.h>
#include <omm/Logical_struct.h>
#include <omm/Visitor.h>
#include <exception/Exceptions.h>


using namespace arolios;
using namespace arolios::omm;

Logical_schema::Logical_schema() {

}

Logical_schema::~Logical_schema() {

}

std::shared_ptr<Namespace> Logical_schema::own_namespace() const  {
	return nullptr;
}

void Logical_schema::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Logical_schema::type_name () const {
    return "schema";
}

    
void Logical_schema::package (const std::shared_ptr<Package>& p_pk) {
    pk_ = p_pk ;
}

const std::shared_ptr<Package>& Logical_schema::package ()  const {
    return pk_  ;
}

std::unordered_map<std::string,std::shared_ptr<Logical_struct>>& Logical_schema::logical_structs () {
    return structs_;
}


void Logical_schema::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto && elem = std::dynamic_pointer_cast<Logical_struct>(p_elem);
    if (elem) {
        structs_.insert(std::make_pair<const std::string&,const std::shared_ptr<Logical_struct>&>(elem->name(),elem));
    }
}

void Logical_schema::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
    auto search = structs_.find(p_elem->name());
    if ( search != structs_.end() ) {
        structs_.erase (search->first) ;
    } else {
        throw exception::Failure("Logical_struct " + p_elem->name() + " not found" );
    }
}

bool Logical_schema::has_elements (bool p_active_only) const {
    
    //no lc_status, argument p_active_only has no effect
    return structs_.empty();
    

}

void Logical_schema::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
// nothing to do , no namespace
}


std::shared_ptr<Named_element> Logical_schema::find_element ( const std::string& p_name) const {
   auto search = structs_.find(p_name) ;
   return  (search== structs_.end()) ? nullptr : search->second;
}
