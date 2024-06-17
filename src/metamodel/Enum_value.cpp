/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Enum_value.h>
#include <omm/Visitor.h>
#include <omm/Enumeration.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::omm;

Enum_value::Enum_value() {

}

Enum_value::~Enum_value() {

}
    
std::shared_ptr<Namespace> Enum_value::own_namespace() const {
    return enumeration ();
}

std::shared_ptr<Package> Enum_value::package() const {
    return (enum_) ?  enum_->package() : nullptr;
}

void Enum_value::accept (Visitor& p_visitor) const {
    p_visitor.visit ( *this) ;
}

    
void Enum_value::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {

    std::runtime_error e("an enumeration value cannot be inserted directly in a package");
    throw e;
}


void Enum_value::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    // nothing to do
}

void Enum_value::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    //nothing to do

}
    
const std::shared_ptr<Enumeration>& Enum_value::enumeration () const {
    return enum_;
}

void Enum_value::enumeration (const std::shared_ptr<Enumeration>&  p_enum)  {
    enum_ = p_enum;
}


void Enum_value::own_namespace( const std::shared_ptr<Namespace>& p_parent) {
    
    if (!p_parent) {
        enumeration(nullptr);
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Enumeration> (p_parent);
        if (parent) {
            enumeration(parent);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
}

void Enum_value::rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) {
// nothing to do, no element
}
