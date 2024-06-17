/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Logical_enum.h>
#include <omm/Visitor.h>
#include <omm/Enumeration.h>
#include <iostream>



using namespace arolios;
using namespace arolios::omm;

Logical_enum::Logical_enum() {

}

Logical_enum::~Logical_enum() {

}

std::vector<std::shared_ptr<Logical_field>> Logical_enum::fields () const {
    std::vector<std::shared_ptr<Logical_field>> v ;
    return v;
}

std::vector<std::shared_ptr<Classifier>> Logical_enum::classifiers () const  {
    std::vector<std::shared_ptr<Classifier>> v;
    v.push_back(enum_);
    return v;
}

void Logical_enum::insert_element (const std::shared_ptr<Named_element>& p_elem) {

    std::cout << "WARNING: logical_enum is not composed of named elements" << std::endl;
}


void Logical_enum::enumeration (const std::shared_ptr<Enumeration>& p_enum) {
    enum_ = p_enum ;
}

const std::shared_ptr<Enumeration>& Logical_enum::enumeration() const {
    return enum_  ;
}

void Logical_enum::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Logical_enum::type_name () const {
    return "enumeration";
}


std::unordered_set<std::string>& Logical_enum::values ()  {
    return values_;
}


std::shared_ptr<Named_element> Logical_enum::find_element ( const std::string& p_name) const {

    std::cout << "WARNING: logical_enum is not composed of named elements" << std::endl;
    return nullptr;
}

void Logical_enum::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
// Nothing to do
}
