/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>

namespace arolios {

namespace omm {
    class Collection_attributes;
}

namespace model {

    class Collection_t;

    class Collection {

    public: 

        static std::shared_ptr<omm::Collection_attributes> parse_collection (const Collection_t& p_xml_collection);
    };


}
}
