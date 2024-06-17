/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Table.h>
#include <omm/Logical_field.h>
#include <omm/Visitor.h>
#include <omm/Column.h>
#include <omm/Class.h>
#include <exception/Exceptions.h>




using namespace arolios;
using namespace arolios::omm;

Table::Table() {

}

Table::~Table() {

}

std::vector<std::shared_ptr<Logical_field>> Table::fields () const {
    std::vector<std::shared_ptr<Logical_field>> v ;
    for (auto [first, second]: columns_) {
        v.push_back(second);
    }

    return v;
}

std::vector<std::shared_ptr<Classifier>> Table::classifiers () const  {
    std::vector<std::shared_ptr<Classifier>> v;
    for (auto && cls: classes_) {
        v.push_back(cls);
    }
    return v;
}


void Table::association (const std::shared_ptr<Association>& p_assoc) {
    assoc_ = p_assoc ;
}

void Table::insert_element (const std::shared_ptr<Named_element>& p_elem) {
    auto && col = std::dynamic_pointer_cast<Column>(p_elem);
    if (col) {
        columns_.insert(std::make_pair<const std::string&,const std::shared_ptr<Column>&>(col->name(),col));
    }
}

void Table::erase_element (const std::shared_ptr<Named_element>& p_elem) {
    
    auto search = columns_.find(p_elem->name());
    if ( search != columns_.end() ) {
        columns_.erase (search->first) ;
    } else {
        throw exception::Failure("Column " + p_elem->name() + " not found" );
    }
}


std::shared_ptr<Named_element> Table::find_element ( const std::string& p_name) const {
   auto search = columns_.find(p_name) ;
   return  (search== columns_.end()) ? nullptr : search->second;
}


const std::shared_ptr<Association>& Table::association () const {
    return assoc_  ;
}

void Table::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Table::type_name () const {
    return "table";
}

std::unordered_map<std::string,std::shared_ptr<Column>>& Table::columns () {
    return columns_;
}


std::vector<std::shared_ptr<Class>>& Table::classes () {
    return classes_;
}

const std::shared_ptr<Classifier>& Table::root_classifier () const {
    return root_cls_;
}

void Table::root_classifier (const std::shared_ptr<Classifier>& p_cls) {
    root_cls_ = p_cls;
}


