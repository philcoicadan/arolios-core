/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <mttr/Instance_field.h>
#include <sstream>
#include <util/String.h>
#include <qry/Util.h>


using namespace arolios;
using namespace arolios::mttr;

Instance_field::Instance_field(const std::string& p_column_name, const util::Native_type p_type, const  std::shared_ptr<const Json::Value>& p_data) 
: column_name_ (p_column_name){
    type_ = p_type;
    data_ = p_data;

}

Instance_field::~Instance_field() {

}

std::string Instance_field::column_full_name() const {
    return column_name_;
}

std::string Instance_field::value_expression () const {
    return qry::Util::json_to_native(type_, *data_) ;

}
