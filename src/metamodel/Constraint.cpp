/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Constraint.h>
#include <omm/Package.h>
#include <omm/Visitor.h>


using namespace arolios;
using namespace arolios::omm;


Constraint::Constraint () { namespace_ = nullptr;  }

std::shared_ptr<Namespace> Constraint::own_namespace() const
{
	return namespace_;
};

std::shared_ptr<Package> Constraint::package() const {
    //TODO temporary implementation
    return std::dynamic_pointer_cast<Package>(namespace_);
}

void Constraint::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

void Constraint::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    //TODO temporary implementation
    std::runtime_error e("a constraint cannot be inserted directly in a package");
    throw e;
}


void Constraint::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    // nothing to do
}

void Constraint::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{  // nothing to do
}
    	
void Constraint::rename_element ( const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name) 
{  // nothing to do, no element
}