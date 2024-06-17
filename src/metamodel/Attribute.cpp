/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Attribute.h>
#include <omm/Visitor.h>


using namespace arolios;
using namespace arolios::omm;


Attribute::Attribute () {   }

Attribute::~Attribute () {}




void Attribute::accept (Visitor& p_visitor) const {
	p_visitor.visit(*this);
}

std::string Attribute::type_name () const {
    return "attribute";
}


    
