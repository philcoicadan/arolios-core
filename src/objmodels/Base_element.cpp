/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Base_element.h>


using namespace arolios;
using namespace arolios::model;

Base_element::Base_element( const std::string& p_name, const Element_action p_action) : std_name_(p_name), action_(p_action){


}

Base_element::~Base_element() {
}

void Base_element::execute_children ()  {

	fill_children() ;

	for (std::vector<std::shared_ptr<Base_element>>::const_iterator i(get_children().begin()); i != get_children().end(); ++i ) {
		(*i)->execute();
	}
}



std::shared_ptr<Base_element> Base_element::find_child (const std::string& p_name) const {
    auto search = map_.find( p_name) ;
    if (search != map_.end()) {
        return search->second;
    } else {
        return nullptr;
    }
}

void Base_element::add_child (const std::shared_ptr<Base_element>& p_elem) {
    map_.insert( {p_elem->standard_name(), p_elem});
    vec_.push_back(p_elem);
}
