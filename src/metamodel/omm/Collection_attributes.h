/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include "Collection_kind.h"
#include <vector>

namespace arolios {


namespace omm {

struct Collection_range {
    long min_ {0};
    long max_ {1}; // -1 means many
};

class Collection_attributes {
public:

    virtual ~Collection_attributes () {};
    Collection_attributes () = default;
    Collection_attributes ( const Collection_attributes& ) = default;

    virtual void kind(const Collection_kind p_kind);
    virtual Collection_kind kind() const;

    virtual void add_range (const long p_min, const long p_max);
    virtual void ranges (const std::vector<Collection_range>& p_ranges);
    virtual const std::vector<Collection_range>& ranges () const;

private:

    Collection_kind kind_ { Collection_kind::array };
    std::vector<Collection_range> ranges_;

};
}
 

}
