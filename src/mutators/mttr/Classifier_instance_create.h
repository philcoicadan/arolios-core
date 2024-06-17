/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <mttr/Base_element_create.h>
#include <omm/Classifier.h>

namespace arolios {



namespace mttr {
class Classifier_instance_create : public Base_element_create {
public:
    /**
     * Constructor
     */
    Classifier_instance_create( const std::shared_ptr<const omm::Classifier>& p_csf, const Json::Value& p_data_root, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache);

    /**
     * Destructor
     */
    virtual ~Classifier_instance_create();
    
     virtual void execute () override ;
    

    
private:

    std::shared_ptr<const omm::Classifier> csf_ ;

    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache_;


};
}

}

