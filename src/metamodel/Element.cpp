/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Element.h>




using namespace arolios;
using namespace arolios::omm;


        
const std::string& Element::description () const 
{
    return description_;
}

void Element::description (const std::string& p_desc)  {
    description_ = p_desc;
}


