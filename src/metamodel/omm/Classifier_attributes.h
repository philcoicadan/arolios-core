/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include <memory>

namespace arolios {



namespace omm {
    class Classifier;

 class Classifier_attributes : public Element_attributes {
 public:
     Classifier_attributes () ;
     virtual ~Classifier_attributes ();
     bool is_abstract_;
     std::shared_ptr<omm::Classifier> base_ ;
} ;
}


}

