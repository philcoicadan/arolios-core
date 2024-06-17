/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <mttr/Base_element_update.h>
#include <omm/Association.h>

namespace arolios {


namespace mttr {
class Class_instance_association_update : public Base_element_update {
public:
    /**
     * Constructor
     */
    Class_instance_association_update( const std::shared_ptr<const omm::Association>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~Class_instance_association_update();
    
     virtual void execute () override ;

private:

    std::shared_ptr<const omm::Association> assoc_ ;

    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache_;


};
}
}

