/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Element_attributes.h"
#include <memory>
#include <vector>

namespace arolios {

namespace omm {
class Classifier;

class Selection_attributes : public Element_attributes {
public:
  Selection_attributes();
  virtual ~Selection_attributes();
  std::vector<std::shared_ptr<Classifier>> options_;
};
} // namespace omm

} // namespace arolios
