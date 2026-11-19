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
    class Classifier;
    class Collection_attributes;
    
  class Specific_attributes : public Element_attributes {
 public:
      Specific_attributes ();
      virtual ~Specific_attributes ();
      std::shared_ptr<Classifier> type_ ;
      std::shared_ptr<Collection_attributes> collection_;

 } ;
}


}

