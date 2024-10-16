/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <memory>
#include <string>
#include <json/json.h>
#include <mttr/Classifier_instance_update.h>

namespace arolios {




namespace omm {
    class Classifier;
}

namespace mttr {
class Classifier_instance_wlo_update : public Classifier_instance_update {
public:
    /**
     * Constructor
     */
    Classifier_instance_wlo_update( const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::string& p_inst_id, const int p_rfu_count = 0, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache = {});

    /**
     * Destructor
     */
    virtual ~Classifier_instance_wlo_update();
 
    virtual void execute () override;

private:

    std::shared_ptr<const omm::Classifier> csf_ ;

    unsigned long row_id_;
    unsigned int csf_id_;


};
}
}

