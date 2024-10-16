/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <mttr/Classifier_instance_update.h>
#include <omm/Association.h>

namespace arolios {


namespace mttr {
class Association_instance_wolo_update : public Classifier_instance_update {
public:
    /**
     * Constructor
     */
    Association_instance_wolo_update( const std::shared_ptr<const omm::Association>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count = 0, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache= {});

    /**
     * Destructor
     */
    virtual ~Association_instance_wolo_update();
    
     virtual void execute () override ;

private:

    std::shared_ptr<const omm::Association> assoc_ ;

};
}
}

