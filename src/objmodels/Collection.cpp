/*
 *
 * Copyright (C) 2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#include <model/Collection.h>
#include <model/xam.h>
#include <omm/Collection_attributes.h>
#include <exception/Exceptions.h>
#include <string>

using namespace arolios;
using namespace arolios::model;

std::shared_ptr<omm::Collection_attributes> Collection::parse_collection (const Collection_t& p_xml_collection) {

    if (p_xml_collection.range().empty()) {
        throw exception::Model_error("collection must contain at least one range");
    }

    auto result = std::make_shared<omm::Collection_attributes>();

    switch (static_cast<KindOfCollection_t::value>(p_xml_collection.kind())) {
        case KindOfCollection_t::array:
            result->kind(omm::Collection_kind::array);
            break;
        case KindOfCollection_t::list:
            result->kind(omm::Collection_kind::list);
            break;
        case KindOfCollection_t::set:
            result->kind(omm::Collection_kind::set);
            break;
        case KindOfCollection_t::bag:
            result->kind(omm::Collection_kind::bag);
            break;
    }

    for (const auto& range : p_xml_collection.range()) {
        const long min = static_cast<long>(range.min());
        long max = -1;

        if (range.max() != "many") {
            max = std::stol(range.max().c_str());
        }

        if (max != -1 && max < min) {
            throw exception::Model_error("collection range max must be greater or equal to min");
        }

        result->add_range(min, max);
    }

    return result;
}
