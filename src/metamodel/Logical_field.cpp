/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Logical_field.h>
#include <omm/Logical_struct.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::omm;

Logical_field::Logical_field() {
    native_type_ = -1; // default = no native type. if positive interger  => mapped with a native type
}

Logical_field::~Logical_field() {

}

std::shared_ptr<Namespace> Logical_field::own_namespace() const {
    return struct_;
}
    
const std::shared_ptr<Logical_struct>& Logical_field::logical_struct () const {
    return struct_;
}
    
void Logical_field::logical_struct ( const std::shared_ptr<Logical_struct>& p_struct ) {
    struct_ = p_struct;
}

void Logical_field::native_type (const int p_type ) {
    native_type_ = p_type;
}

void Logical_field::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        logical_struct(nullptr);
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Logical_struct> (p_parent);
        if (parent) {
            logical_struct(parent);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}

    
const int  Logical_field::native_type() const {
    return native_type_;
}

const std::shared_ptr<Property>& Logical_field::property () const {
    return property_;
}

void Logical_field::property ( const std::shared_ptr<Property>& p_prop )  {
    property_ = p_prop;
}

std::string Logical_field::full_name () const {
    return logical_struct()->full_name() + "." + name() ;
}


