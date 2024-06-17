/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Element_update.h>






using namespace arolios;
using namespace arolios::model;

Element_update::Element_update(): new_name_(std::string()) {
	

}

Element_update::~Element_update() {

}


const std::string& Element_update::new_name() const {
    return new_name_;
}

