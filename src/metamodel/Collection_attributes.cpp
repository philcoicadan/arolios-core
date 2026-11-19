/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <omm/Collection_attributes.h>

using namespace arolios;
using namespace arolios::omm;

void Collection_attributes::kind(const Collection_kind p_kind) {
    kind_ = p_kind;
}

Collection_kind Collection_attributes::kind() const {
    return kind_;
}

void Collection_attributes::add_range (const long p_min, const long p_max) {
    ranges_.push_back({p_min, p_max});
}

void Collection_attributes::ranges (const std::vector<Collection_range>& p_ranges) {
    ranges_ = p_ranges;
}

const std::vector<Collection_range>& Collection_attributes::ranges () const {
    return ranges_;
}
