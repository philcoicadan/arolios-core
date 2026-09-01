/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Packageable_element.h>
#include <omm/Package.h>


using namespace arolios;
using namespace arolios::omm;


Lc_status Packageable_element::lc_status() const  {
    return lc_status_;
}


void Packageable_element::lc_status ( const Lc_status p_status ) {
    lc_status_ = p_status;
}

void Packageable_element::recursive_lc_status(const omm::Lc_status p_status)
{

    lc_status ( p_status );
}
