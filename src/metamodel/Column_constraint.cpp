/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Column_constraint.h>


using namespace arolios;
using namespace arolios::omm;

Column_constraint::Column_constraint( const std::shared_ptr<Column>& p_column,  Type p_type):
column_(p_column), type_(p_type) {

}

Column_constraint::~Column_constraint() {

}


const std::shared_ptr<Column>& Column_constraint::column() const {
    return column_;
}

Column_constraint::Type Column_constraint::type() const {
    return type_;
}

    
