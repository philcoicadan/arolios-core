/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Column.h>
#include <omm/Table.h>
#include <omm/Visitor.h>
#include <omm/Column_constraint.h>


using namespace arolios;
using namespace arolios::omm ;

Column::Column() {

}

Column::~Column() {

}


void Column::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Column::type_name () const {
    return "column";
}


const std::vector<std::shared_ptr<Column_constraint>>& Column::constraints()  const {
    return constraints_  ;
}


void Column::insert_constraint (const std::shared_ptr<Column_constraint>& p_ctt)  {
    constraints_.push_back(p_ctt);
    
}

void Column::erase_constraint ( const Column_constraint::Type ctt_type) {
    
    for ( auto it = constraints_.begin(); it != constraints_.end() ; ++it) {
        if ((*it)->type() == ctt_type) {
            constraints_.erase (it) ;
            break;
        }
    }
}

const std::shared_ptr<Table> Column::table() const {
    const auto & ls = logical_struct();
    return std::dynamic_pointer_cast<Table>(ls) ;
}

