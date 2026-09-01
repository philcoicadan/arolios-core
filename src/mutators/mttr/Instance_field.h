/*
 *
 * Copyright (C) 2024-2026 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <util/Native_type.h>
#include <memory>

namespace arolios {

namespace mttr {
class Instance_field {
public:
    /**
     * Constructor
     */
    Instance_field( const std::string& p_column_name, const util::Native_type p_type, const std::shared_ptr<const Json::Value>& p_data);

    /**
     * Destructor
     */
    ~Instance_field();
    
    std::string column_full_name () const;
    std::string value_expression () const;
    
private:

    std::string column_name_;
    util::Native_type type_;
    std::shared_ptr<const Json::Value> data_;


};

}
}

