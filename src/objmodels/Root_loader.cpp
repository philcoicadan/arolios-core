/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Root_loader.h>
#include <omm/Package.h>
#include <model/Visitor.h>


using namespace arolios;
using namespace arolios::model;

Root_loader::Root_loader( const std::shared_ptr<omm::Packageable_element>& p_elem) : Base_element ("model") , root_elem_ (p_elem) { }

Root_loader::~Root_loader() {}

void Root_loader::execute () {}

const std::shared_ptr<omm::Packageable_element> Root_loader::stored_element () const  { 
    return root_elem_; 
    
}

void Root_loader::accept(Visitor& p_visitor) {
    p_visitor.visit (*this) ;
}


void Root_loader::fill_children () {}

void Root_loader::internal_execute (const omm::Element_attributes& p_attrs)  {}

