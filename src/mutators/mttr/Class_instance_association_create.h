/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <mttr/Base_element_create.h>
#include <omm/Association.h>
#include <common/Db_client.h>

namespace arolios {



namespace mttr {
class Class_instance_association_create : public Base_element_create {
public:
    /**
     * Constructor
     */
    Class_instance_association_create( const std::shared_ptr<const omm::Association>& p_csf, const Json::Value& p_data_root, const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& p_import_cache);

    /**
     * Destructor
     */
    virtual ~Class_instance_association_create();
    
    virtual void execute () override ;
    
    //std::string association_id () const ;
    

private:

    std::shared_ptr<const omm::Association> assoc_ ;

    unsigned long upd_row_id_;
    unsigned long new_upd_count_;
    const std::unordered_map<std::string, std::shared_ptr<const Json::Value>>& import_cache_;

};
}
}

