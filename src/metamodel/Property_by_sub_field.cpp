/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Property_by_sub_field.h>
#include <omm/Visitor.h>


using namespace arolios;
using namespace arolios::omm;

Property_by_sub_field::Property_by_sub_field() {

}

Property_by_sub_field::~Property_by_sub_field ( ) {

}


void Property_by_sub_field::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Property_by_sub_field::type_name () const {
    return "sub_field";
}

const std::shared_ptr<Column>& Property_by_sub_field::filtering_column() const {
    return fk_column_;
}
    
void Property_by_sub_field::filtering_column ( const std::shared_ptr<Column>& p_column) {
    fk_column_ = p_column;
}

const std::shared_ptr<Property>& Property_by_sub_field::property () const {
    return property_;
}

void Property_by_sub_field::property ( const std::shared_ptr<Property>& p_prop )  {
    property_ = p_prop;
}


