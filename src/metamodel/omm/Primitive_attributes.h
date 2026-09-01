/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include "util/Native_type.h"


namespace arolios {



namespace omm {
    class Classifier;
    class Class;
    
 class Primitive_attributes : public Element_attributes {
 public:
    Primitive_attributes ();
    virtual ~Primitive_attributes ();
    util::Native_type native_type_;
} ;
}


}

