/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include "Aggregation_kind.h"
#include <memory>

namespace arolios {



namespace omm {
    class Classifier;
    class Class;
    
 class Property_attributes : public Element_attributes {
 public:
     Property_attributes ();
     virtual ~Property_attributes ();
     long min_ ;
     long max_ ;
     Aggregation_kind aggregation_ ;
     std::shared_ptr<Classifier> type_ ;

     std::shared_ptr<Class> source_member_end_;
     bool is_scp_;
} ;
}


}

