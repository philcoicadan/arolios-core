/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <mttr/Base_element_update.h>

namespace arolios {



namespace mttr {
class Classifier_instance_update : public Base_element_update {
public:
    /**
     * Constructor
     */
    Classifier_instance_update( const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count = 0, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache = {});

    /**
     * Destructor
     */
    virtual ~Classifier_instance_update();
    
protected:

    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache_;

};
}

}

