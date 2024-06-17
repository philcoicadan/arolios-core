/*
 *
 * Copyright (C) 2024 Philippe Coicadan. All rights reserved.
 * License: LGPL-3.0-or-later
 *
 */
#pragma once
#include <json/json.h>
#include <common/Db_client.h>
#include <mttr/Base_element_create.h>

namespace arolios {




namespace mttr {
class App_user_create : public Base_element_create {
public:
    /**
     * Constructor
     */
    App_user_create(const Json::Value& p_data_root);

    /**
     * Destructor
     */
    virtual ~App_user_create();
    
  virtual void execute () override;
    
    
  

    
private:

std::string retrieve_user_internal_id (const std::string& p_name) const ;



};
}
}

