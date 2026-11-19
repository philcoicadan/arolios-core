/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include <memory>

namespace arolios {



namespace omm {
    class Constraint;

 class Constraint_attributes : public Element_attributes {
 public:
     Constraint_attributes () ;
     virtual ~Constraint_attributes ();
    std::string expression_;
} ;
}


}

