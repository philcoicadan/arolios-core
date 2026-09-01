/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Data_type.h>
#include <omm/Package.h>
#include <omm/Visitor.h>
#include <omm/Logical_type.h>

using namespace arolios;
using namespace arolios::omm;

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

std::string Data_type::read_select_template( const bool p_force) { return ""; }

std::unordered_map<std::string,std::shared_ptr<Property>> Data_type::all_id_properties () const{
    return std::unordered_map<std::string,std::shared_ptr<Property>>();
}

void Data_type::make_more_specific_ids ( const bool p_force) {
    //Nothing to do

}
