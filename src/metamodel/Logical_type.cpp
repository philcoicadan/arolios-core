/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Logical_type.h>
#include <omm/Logical_field.h>
#include <omm/Visitor.h>
#include <omm/Attribute.h>
#include <omm/Data_type.h>
#include <exception/Exceptions.h>



using namespace arolios;
using namespace arolios::omm;

Logical_type::Logical_type() {

}

Logical_type::~Logical_type() {

}

std::vector<std::shared_ptr<Logical_field>> Logical_type::fields () const {
    std::vector<std::shared_ptr<Logical_field>> v ;
    for (auto && [first, second]: attributes_) {
        v.push_back(second);
    }
    return v;
}

std::vector<std::shared_ptr<Classifier>> Logical_type::classifiers () const  {
    std::vector<std::shared_ptr<Classifier>> v;
    v.push_back(data_type_);
    return v;
}

void Logical_type::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto && elem = std::dynamic_pointer_cast<Attribute>(p_elem);
    if (elem) {
        attributes_.insert(std::make_pair<const std::string&,const std::shared_ptr<Attribute>&>(elem->name(),elem));
    }
}


void Logical_type::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
    auto search = attributes_.find(p_elem->name());
    if ( search != attributes_.end() ) {
        attributes_.erase (search->first) ;
    } else {
        throw exception::Failure("attribute " + p_elem->name() + " not found" );
    }
}

std::shared_ptr<Named_element> Logical_type::find_element ( const std::string& p_name) const {
   auto search = attributes_.find(p_name) ;
   return  (search== attributes_.end()) ? nullptr : search->second;
}


void Logical_type::data_type (const std::shared_ptr<Data_type>& p_type) {
    data_type_ = p_type ;
}

const std::shared_ptr<Data_type>& Logical_type::data_type () const {
    return data_type_  ;
}

void Logical_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Logical_type::type_name () const {
    return "type";
}


const std::unordered_map<std::string,std::shared_ptr<Attribute>>& Logical_type::attributes () const {
    return attributes_;
}

void Logical_type::add_attribute ( const std::shared_ptr<Attribute>& p_attr) {
    attributes_.insert ( {p_attr->name(), p_attr});
}

std::shared_ptr<Attribute> Logical_type::find_attribute ( const std::string& p_name) const {
    auto search = attributes_.find(p_name);
    if ( search != attributes_.end()) {
        return search->second;
    } else {
        return nullptr;
    }
}
