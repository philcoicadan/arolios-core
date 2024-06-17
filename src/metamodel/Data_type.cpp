/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Data_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Logical_type.h>

using namespace arolios;
using namespace arolios::omm;

void Data_type::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::shared_ptr<Logical_struct> Data_type::logical_struct() const {
    return type_;
}

const std::shared_ptr<Logical_type>& Data_type::logical_type() const {
    return type_;
}

void Data_type::logical_type (const std::shared_ptr<Logical_type>& p_type) {
    type_ = p_type;
}

std::shared_ptr<Table> Data_type::table() const {
    return nullptr;
}

std::shared_ptr<Table> Data_type::managing_table() const {
    return nullptr;
}


void Data_type::insert_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.insert<Data_type> (p_elem) ;
}


void Data_type::erase_from_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem) {
    p_pk.erase<Data_type> (p_elem) ;
}

void Data_type::rename_in_package (Package& p_pk, const std::shared_ptr<Packageable_element>& p_elem, const std::string& p_new_name ) 
{
    p_pk.rename<omm::Data_type> (p_elem, p_new_name) ;

}

std::string Data_type::read_select_template( const bool p_force) { return ""; }

std::unordered_map<std::string,std::shared_ptr<Property>> Data_type::all_id_properties () const{
    return std::unordered_map<std::string,std::shared_ptr<Property>>();
}

void Data_type::make_more_specific_ids ( const bool p_force) {
    //Nothing to do

}



const std::string& Data_type::meta_type() const {
    static std::string mt = "type";
    return mt;
}
