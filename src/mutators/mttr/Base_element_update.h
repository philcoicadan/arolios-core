/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <string>
#include <json/json.h>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {






namespace mttr {
class Base_element_update : public qry::Base_query {
public:
    /**
     * Constructor
     */
    Base_element_update(  const std::string& p_elem_id , const Json::Value& p_data_root, const int p_rfu_count = 0);

    /**
     * Destructor
     */
    virtual ~Base_element_update();
 
    
    
    int new_update_counter () const;

    
protected:
    const std::shared_ptr<common::App_info> app_info_ptr_;
    const Json::Value& data_root_;
    std::string elem_id_;

    int rfu_count_;
    int new_upd_count_;

};
}
}

