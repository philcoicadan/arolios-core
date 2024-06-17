/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <qry/Base_query.h>
#include <common/Singleton.h>
#include <common/App_info.h>

namespace arolios {



namespace mttr {
class Base_element_create : public qry::Base_query {
public:
    /**
     * Constructor
     */
    Base_element_create(const Json::Value& p_data_root);

    /**
     * Destructor
     */
    virtual ~Base_element_create();
    

    
  

    
protected:

    std::shared_ptr<common::App_info> app_info_ptr_;
    const Json::Value& data_root_;



};
}
}

