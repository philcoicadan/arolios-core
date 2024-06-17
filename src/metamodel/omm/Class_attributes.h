/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include <memory>

namespace arolios {



namespace omm {
    class Class;
    
 class Class_attributes : public Element_attributes {
 public:
     Class_attributes () ;
     virtual ~Class_attributes ();
     bool is_abstract_;
     std::shared_ptr<omm::Class> base_ ;
} ;
}


}

