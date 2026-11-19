/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
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
    class Collection_attributes;
    
  class Property_attributes : public Element_attributes {
 public:
      Property_attributes ();
      virtual ~Property_attributes ();
      long min_ ;
      long max_ ;
      Aggregation_kind aggregation_ ;
      std::shared_ptr<Classifier> type_ ;

      std::shared_ptr<Classifier> source_member_end_;
      std::shared_ptr<Collection_attributes> collection_;
      bool is_scp_;
      bool is_member_end_;
      std::string opposite_name_;
      bool is_navigable_;
 } ;
}


}

