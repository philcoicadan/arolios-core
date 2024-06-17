/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Logical_struct.h>
#include <omm/Logical_field.h>
#include <omm/Visitor.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::omm;

Logical_struct::Logical_struct() {

}

Logical_struct::~Logical_struct() {

}



std::shared_ptr<Namespace> Logical_struct::own_namespace() const  {
	return schema_;
}

void Logical_struct::schema (const std::shared_ptr<Logical_schema>& p_schema) {
    schema_ = p_schema ;
}

const std::shared_ptr<Logical_schema>& Logical_struct::schema () const {
    return schema_  ;
}

std::string Logical_struct::full_name() const {
    //TODO rule for Postgres. To be configured for other DBMS
    return schema_->name() + "." + name();
}



bool Logical_struct::has_elements (bool p_active_only) const {
    
    //no lc_status, argument p_active_only has no effect
    return fields().empty();
    

}

void Logical_struct::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        schema(nullptr);
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Logical_schema> (p_parent);
        if (parent) {
            schema(parent);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}

