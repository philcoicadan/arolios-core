/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Constraint.h>
#include <omm/Classifier.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <exception/Exceptions.h>


using namespace arolios;
using namespace arolios::omm;


Constraint::Constraint () { classifier_ = nullptr;  }

std::shared_ptr<Namespace> Constraint::own_namespace() const
{
	return classifier();
};

const std::shared_ptr<Classifier>& Constraint::classifier () const {
	return classifier_;
}

void Constraint::classifier (const std::shared_ptr<Classifier>& p_csf) {
	classifier_ = p_csf;
}

std::shared_ptr<Package> Constraint::package() const {
    return (classifier_) ? classifier_->package() : nullptr;
}

void Constraint::own_namespace( const std::shared_ptr<Namespace>& p_parent) {

    if (!p_parent) {
        classifier(nullptr);
    }
    else {
        const auto & parent = std::dynamic_pointer_cast<omm::Classifier> (p_parent);
        if (parent) {
            classifier(parent);
        } else {
            throw exception::Failure ("bad type for parent object");
        }
        
    }
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